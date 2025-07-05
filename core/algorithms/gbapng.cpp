#include "algorithms/gbapng.h"

#include <spng.h>

#include <bit>
#include <cassert>
#include <concepts>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "structviewer/palette.h"
#include "structviewer/tilemap.h"
#include "ui.h"

#define CHECK_STATUS                          \
    ({                                        \
        if (status) {                         \
            UI->error(spng_strerror(status)); \
        }                                     \
    })
static int status {};

static constexpr unsigned BITDEPTH_4BPP {4};  // That's why it's called 4bpp, duh
static constexpr unsigned TILE_PIXELWIDTH {8};
static constexpr unsigned TILE_PIXELHEIGHT {8};
static constexpr unsigned TILE_BYTEWIDTH_4BPP {BITDEPTH_4BPP * TILE_PIXELWIDTH / 8};

static constexpr auto translate_to_4bpp_tiled_image_index {
    [](unsigned y, unsigned x, unsigned max_x) -> unsigned {
        return (x % TILE_BYTEWIDTH_4BPP) +
               (x / TILE_BYTEWIDTH_4BPP) * TILE_BYTEWIDTH_4BPP * TILE_PIXELHEIGHT +
               (y % TILE_PIXELHEIGHT) * TILE_BYTEWIDTH_4BPP +
               (y / TILE_PIXELHEIGHT) * max_x * TILE_PIXELHEIGHT;
    }
};

/* Helper function to encode a png progressively by giving LambdaIntoPngrow two indices with the
   following specification:
 * The first index (rowindex) represents the index of the current row in the png.
 * The second index (columnbyteindex) represents one byte of pngrow, so two pixels with bitdepth 4
   and one pixel with bitdepth 8.
 * The lambda then returns the according pixelvalue from captured datastructures pngrow, such that
   pngrow can afterwards be used to encode a row of the png.

 * ctx must already have all necessary metadata (e.g. idhr, plte) set.
 */
template <typename LambdaGetSrcByte>
    requires requires(LambdaGetSrcByte get_src_byte, unsigned rowindex, unsigned columnbyteindex) {
        { get_src_byte(rowindex, columnbyteindex) } -> std::same_as<unsigned char>;
    }
void static encode_png_by_row(
    spng_ctx* ctx, const unsigned image_pixelheight, unsigned pngrow_size, LambdaGetSrcByte&& get_src_byte
) {
    status = spng_encode_image(ctx, nullptr, 0, SPNG_FMT_PNG, SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);
    CHECK_STATUS;

    std::vector<unsigned char> pngrow(pngrow_size);
    for (unsigned rowindex {}; rowindex < image_pixelheight; ++rowindex) {
        for (unsigned columnbyteindex {}; columnbyteindex < pngrow_size; ++columnbyteindex) {
            pngrow.at(columnbyteindex) =
                std::forward<LambdaGetSrcByte>(get_src_byte)(rowindex, columnbyteindex);
        }
        status = spng_encode_row(ctx, pngrow.data(), pngrow.size());
    }

    if (status != SPNG_EOI) {
        spng_ctx_free(ctx);
        UI->error("PNG Encoding did not finish successfully");
    }
}

auto png::from_4bpp_tiled_image_gray(
    const char tiled_image[], unsigned tilewidth, unsigned tileheight, bool inversed
) -> std::pair<std::unique_ptr<const char[]>, const long> {
    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {
        .width = tilewidth * TILE_PIXELWIDTH,
        .height = tileheight * TILE_PIXELHEIGHT,
        .bit_depth = 4,
        .color_type = SPNG_COLOR_TYPE_GRAYSCALE,
        .compression_method = 0,
        .filter_method = 0,
        .interlace_method = 0
    };
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    encode_png_by_row(
        ctx, tileheight * TILE_PIXELHEIGHT, tilewidth * TILE_BYTEWIDTH_4BPP,
        [&tiled_image, &tilewidth,
         &inversed](const unsigned rowindex, const unsigned columnbyteindex) -> unsigned char {
            unsigned char srcbyte {static_cast<unsigned char>(tiled_image[translate_to_4bpp_tiled_image_index(
                rowindex, columnbyteindex, tilewidth * TILE_BYTEWIDTH_4BPP
            )])};
            if (inversed) {
                srcbyte = 0xff - srcbyte;
            }

            return std::rotl(srcbyte, 4);
        }
    );

    unsigned long pngsize {};
    auto pngbuffer {
        std::unique_ptr<const char[]>(static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, &status)))
    };
    CHECK_STATUS;

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}

auto png::from_4bpp_tiled_image(
    const char tiled_image[], unsigned tilewidth, unsigned tileheight, const PaletteView& palette,
    const TilemapView& tilemap
) -> std::pair<std::unique_ptr<const char[]>, const long> {
    const std::vector<u8>& pngpalette {palette.get_spng_plte()};

    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {
        .width = tilewidth * TILE_PIXELWIDTH,
        .height = tileheight * TILE_PIXELWIDTH,
        .bit_depth = 8,
        .color_type = SPNG_COLOR_TYPE_INDEXED,
        .compression_method = 0,
        .filter_method = 0,
        .interlace_method = 0
    };
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    assert(pngpalette.size() / 4 < 256);
    spng_plte plte {.n_entries = static_cast<uint32_t>(pngpalette.size() / 4), .entries = {}};
    std::memcpy(static_cast<spng_plte_entry*>(plte.entries), pngpalette.data(), pngpalette.size());
    status = spng_set_plte(ctx, &plte);
    CHECK_STATUS;

    encode_png_by_row(
        ctx, tileheight * TILE_PIXELHEIGHT, tilewidth * TILE_PIXELWIDTH,
        [&tiled_image, &tilewidth, &tilemap](unsigned rowindex, unsigned columnbyteindex) -> unsigned char {
            unsigned srcindex {translate_to_4bpp_tiled_image_index(
                rowindex, columnbyteindex / 2, tilewidth * TILE_BYTEWIDTH_4BPP
            )};
            return ((static_cast<unsigned char>(tiled_image[srcindex]) >> ((columnbyteindex % 2) * 4)) & 0x0f
                   ) |
                   tilemap.get_tile_palette_shifted(srcindex / (TILE_PIXELHEIGHT * TILE_BYTEWIDTH_4BPP)) << 4;
        }
    );

    unsigned long pngsize {};
    auto pngbuffer {
        std::unique_ptr<const char[]>(static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, &status)))
    };
    CHECK_STATUS;

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}
