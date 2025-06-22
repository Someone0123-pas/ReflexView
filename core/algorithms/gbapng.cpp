#include "algorithms/gbapng.h"

#include <spng.h>

#include <bit>
#include <cassert>
#include <concepts>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "ui.h"

#define CHECK_STATUS                                      \
    ({                                                    \
        if (status) { UI->error(spng_strerror(status)); } \
    })
static int status {};

static constexpr unsigned BITDEPTH_4BPP {4};  // That's why it's called 4bpp, duh
static constexpr unsigned TILE_PIXELWIDTH {8};
static constexpr unsigned TILE_PIXELHEIGHT {8};
static constexpr unsigned TILE_PIXELNUM {TILE_PIXELWIDTH * TILE_PIXELHEIGHT};
static constexpr unsigned TILE_BYTEWIDTH_4BPP {BITDEPTH_4BPP * TILE_PIXELWIDTH / 8};

/* Helper function to encode a png progressively by giving LambdaIntoPngrow two indices with the
   following specification:
 * The first index (rowindex) represents the index of the current row in the png.
 * The second index (columnbyteindex) represents one byte of pngrow, so two pixels with bitdepth 4
   and one pixel with bitdepth 8.
 * The lambda then returns the according pixelvalue from captured datastructures pngrow, such that
   pngrow can afterwards be used to encode a row of the png.

 * ctx must already have all necessary metadata (e.g. idhr, plte) set.

 * pngrow must be sized to accomodate exactly one row of a previously chosen bitdepth (in ihdr)
 */
template <typename LambdaGetSrcByte>
    requires requires(LambdaGetSrcByte get_src_byte, unsigned rowindex, unsigned columnbyteindex) {
        { get_src_byte(rowindex, columnbyteindex) } -> std::same_as<unsigned char>;
    }
void static encode_png_by_row(spng_ctx* ctx, const unsigned height_in_pixel, unsigned pngrow_size,
                              LambdaGetSrcByte&& get_src_byte) {
    status = spng_encode_image(ctx, nullptr, 0, SPNG_FMT_PNG,
                               SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);
    CHECK_STATUS;

    std::vector<unsigned char> pngrow(pngrow_size);
    for (unsigned rowindex {}; rowindex < height_in_pixel; ++rowindex) {
        for (unsigned columnbyteindex {}; columnbyteindex < pngrow.size(); ++columnbyteindex) {
            pngrow.at(columnbyteindex) = get_src_byte(rowindex, columnbyteindex);
        }
        status = spng_encode_row(ctx, pngrow.data(), pngrow.size());
    }

    if (status != SPNG_EOI) {
        spng_ctx_free(ctx);
        UI->error("PNG Encoding did not finish successfully");
    }
}

auto png::from_4bpp_tiled_image_gray(const std::shared_ptr<const char[]>& tiled_image,
                                     unsigned tilewidth, unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, const long> {
    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {.width = tilewidth * TILE_PIXELWIDTH,
                    .height = tileheight * TILE_PIXELHEIGHT,
                    .bit_depth = 4,
                    .color_type = SPNG_COLOR_TYPE_GRAYSCALE,
                    .compression_method = 0,
                    .filter_method = 0,
                    .interlace_method = 0};
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    encode_png_by_row(
        ctx, tileheight * TILE_PIXELHEIGHT, tilewidth * TILE_BYTEWIDTH_4BPP,
        [&tiled_image, &tilewidth, &inversed](const unsigned rowindex,
                                              const unsigned columnbyteindex) -> unsigned char {
            char srcbyte {(tiled_image[(columnbyteindex % TILE_BYTEWIDTH_4BPP) +
                                       (columnbyteindex / TILE_BYTEWIDTH_4BPP) *
                                           TILE_BYTEWIDTH_4BPP * TILE_PIXELHEIGHT +
                                       (rowindex % TILE_PIXELHEIGHT) * TILE_BYTEWIDTH_4BPP +
                                       (rowindex / TILE_PIXELHEIGHT) * TILE_BYTEWIDTH_4BPP *
                                           tilewidth * TILE_PIXELHEIGHT])};

            if (inversed) { srcbyte = static_cast<char>(~srcbyte); }

            return std::rotl(static_cast<unsigned char>(srcbyte), 4);
        });

    unsigned long pngsize {};
    auto pngbuffer {std::unique_ptr<const char[]>(
        static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, &status)))};
    CHECK_STATUS;

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}

// TODO: Pass TilemapView and PaletteView as arguments
auto png::from_4bpp_tiled_image(const std::shared_ptr<const char[]>& tiled_image,
                                unsigned tilewidth, unsigned tileheight,
                                const std::vector<u8>& pngpalette)
    -> std::pair<std::unique_ptr<const char[]>, const long> {
    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {.width = tilewidth * TILE_PIXELWIDTH,
                    .height = tileheight * TILE_PIXELWIDTH,
                    .bit_depth = 8,
                    .color_type = SPNG_COLOR_TYPE_INDEXED,
                    .compression_method = 0,
                    .filter_method = 0,
                    .interlace_method = 0};
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    assert(pngpalette.size() / 4 < 256);
    spng_plte plte {.n_entries = static_cast<uint32_t>(pngpalette.size() / 4), .entries = {}};
    std::memcpy(plte.entries, pngpalette.data(), pngpalette.size());
    status = spng_set_plte(ctx, &plte);
    CHECK_STATUS;

    // TODO: Call encode from 8x8 tiles, get buffer and return

    return {};
}
