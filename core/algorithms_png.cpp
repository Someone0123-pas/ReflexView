#include <spng.h>

#include <bit>
#include <memory>
#include <utility>
#include <vector>

#include "algorithms.h"

auto png::from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long> {
    constexpr unsigned BITDEPTH {4};  // That's why it's called 4bpp, duh
    constexpr unsigned TILE_PIXELWIDTH {8};
    constexpr unsigned TILE_PIXELHEIGHT {8};

    int status {};

    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);

    spng_ihdr ihdr {
        .width = tilewidth * TILE_PIXELWIDTH,
        .height = tileheight * TILE_PIXELWIDTH,
        .bit_depth = BITDEPTH,
        .color_type = SPNG_COLOR_TYPE_GRAYSCALE,  // TODO: Change to indexed with dedicated palettes
        .compression_method = 0,
        .filter_method = 0,
        .interlace_method = 0};
    spng_set_ihdr(ctx, &ihdr);

    // As 4bpp tilesets are sequences of 8x8 tiles, rather than direct sequences of pixels, the rows
    // must be arranged with additional logic and then encoded progressively

    spng_encode_image(ctx, nullptr, 0, SPNG_FMT_PNG,
                      SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);

    const unsigned BYTES_PER_TILEWIDTH {BITDEPTH * TILE_PIXELWIDTH / 8};
    const unsigned BYTES_PER_TILEROW {BYTES_PER_TILEWIDTH * tilewidth * TILE_PIXELHEIGHT};
    std::vector<unsigned char> byterow(tilewidth * BYTES_PER_TILEWIDTH);

    for (unsigned tilerow {}; tilerow < tileheight; ++tilerow) {
        for (unsigned pixelrow {}; pixelrow < TILE_PIXELHEIGHT; ++pixelrow) {
            for (unsigned byteindex {}; byteindex < tilewidth * BYTES_PER_TILEWIDTH; ++byteindex) {
                unsigned srcindex =
                    (byteindex / BYTES_PER_TILEWIDTH) * BYTES_PER_TILEWIDTH * TILE_PIXELHEIGHT +
                    (byteindex % 4) + pixelrow * BYTES_PER_TILEWIDTH + tilerow * BYTES_PER_TILEROW;

                // The 4bpp entries are little endian
                byterow[byteindex] = std::rotl(static_cast<unsigned char>(src[srcindex]), 4);
            }
            status = spng_encode_row(ctx, byterow.data(), byterow.size());
        }
    }

    if (status != SPNG_EOI) UI->error("PNG Encoding did not return successfully");

    unsigned long pngsize {};
    auto pngbuffer {std::unique_ptr<const char[]>(
        static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, nullptr)))};

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}
