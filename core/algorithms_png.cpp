#include <spng.h>

#include <bit>
#include <memory>
#include <utility>
#include <vector>

#include "algorithms.h"

#define CHECK_STATUS                                      \
    ({                                                    \
        if (status) { UI->error(spng_strerror(status)); } \
    })
static int status {};

static constexpr unsigned BITDEPTH_4BPP {4};  // That's why it's called 4bpp, duh
static constexpr unsigned TILE_PIXELWIDTH {8};
static constexpr unsigned TILE_PIXELHEIGHT {8};

void static encode_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                                unsigned tileheight, bool inversed, spng_ctx* ctx) {
    // As 4bpp tilesets are sequences of 8x8 tiles, rather than direct sequences of pixels, the rows
    // must be arranged with additional logic and then encoded progressively
    status = spng_encode_image(ctx, nullptr, 0, SPNG_FMT_PNG,
                               SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);
    CHECK_STATUS;

    const unsigned BYTES_PER_TILEWIDTH {BITDEPTH_4BPP * TILE_PIXELWIDTH / 8};
    const unsigned BYTES_PER_TILEROW {BYTES_PER_TILEWIDTH * tilewidth * TILE_PIXELHEIGHT};
    std::vector<unsigned char> byterow(tilewidth * BYTES_PER_TILEWIDTH);

    for (unsigned tilerow {}; tilerow < tileheight; ++tilerow) {
        for (unsigned pixelrow {}; pixelrow < TILE_PIXELHEIGHT; ++pixelrow) {
            for (unsigned byteindex {}; byteindex < byterow.size(); ++byteindex) {
                unsigned char srcbyte {static_cast<unsigned char>(
                    src[(byteindex / BYTES_PER_TILEWIDTH) * BYTES_PER_TILEWIDTH * TILE_PIXELHEIGHT +
                        (byteindex % 4) + pixelrow * BYTES_PER_TILEWIDTH +
                        tilerow * BYTES_PER_TILEROW])};

                if (inversed) { srcbyte = ~srcbyte; }

                // The 4bpp entries are little endian
                byterow.at(byteindex) = std::rotl(srcbyte, 4);
            }
            status = spng_encode_row(ctx, byterow.data(), byterow.size());
        }
    }

    if (status != SPNG_EOI) {
        spng_ctx_free(ctx);
        UI->error("PNG Encoding did not finish successfully");
    }
}

auto png::from_4bpp_tileset_gray(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                                 unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long> {
    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {
        .width = tilewidth * TILE_PIXELWIDTH,
        .height = tileheight * TILE_PIXELWIDTH,
        .bit_depth = BITDEPTH_4BPP,
        .color_type = SPNG_COLOR_TYPE_GRAYSCALE,  // TODO: Change to indexed with dedicated palettes
        .compression_method = 0,
        .filter_method = 0,
        .interlace_method = 0};
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    encode_4bpp_tileset(src, tilewidth, tileheight, inversed, ctx);

    unsigned long pngsize {};
    auto pngbuffer {std::unique_ptr<const char[]>(
        static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, &status)))};
    CHECK_STATUS;

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}

auto png::from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight, const PaletteView& palette)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long> {
    return {};
}
