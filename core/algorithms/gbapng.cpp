#include "algorithms/gbapng.h"

#include <spng.h>

#include <bit>
#include <cassert>
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
static constexpr unsigned BYTES_PER_TILEWIDTH_4BPP {BITDEPTH_4BPP * TILE_PIXELWIDTH / 8};

/* Helper function to encode a png progressively by giving Lambda_translate_into_pngrow_pixel a
 linear index of the row (column_4bpp) and an additional translated index, as if the tile pixels
 were stored in GBA VRAM

 * Parametres:
 * ctx must already have all necessary metadata (idhr, plte) set
 * pngrow must be sized to accomodate exactly one row of a previously chosen bitdepth (in ihdr)
 * src_into_pngrow takes the current column_4bpp and srcindex and puts bytes from captured data
 structures (e.g. a src buffer) with the right format into pngrow
 */
template <typename Lambda_translate_into_pngrow_pixel>
void static encode_from_8x8_tiles(
    const std::shared_ptr<const char[]>& src, unsigned tilewidth, unsigned tileheight,
    spng_ctx* ctx, std::vector<unsigned char>& pngrow,
    Lambda_translate_into_pngrow_pixel&& translate_into_pngrow_pixel) {
    status = spng_encode_image(ctx, nullptr, 0, SPNG_FMT_PNG,
                               SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);
    CHECK_STATUS;

    const unsigned BYTES_PER_TILEROW_4BPP {BYTES_PER_TILEWIDTH_4BPP * tilewidth * TILE_PIXELHEIGHT};

    for (unsigned tilerow {}; tilerow < tileheight; ++tilerow) {
        for (unsigned pixelrow {}; pixelrow < TILE_PIXELHEIGHT; ++pixelrow) {
            for (unsigned column_4bpp {}; column_4bpp < tilewidth * BYTES_PER_TILEWIDTH_4BPP;
                 ++column_4bpp) {
                translate_into_pngrow_pixel(
                    column_4bpp, src[(column_4bpp / BYTES_PER_TILEWIDTH_4BPP) *
                                         BYTES_PER_TILEWIDTH_4BPP * TILE_PIXELHEIGHT +
                                     (column_4bpp % 4) + pixelrow * BYTES_PER_TILEWIDTH_4BPP +
                                     tilerow * BYTES_PER_TILEROW_4BPP]);
            }
            status = spng_encode_row(ctx, pngrow.data(), pngrow.size());
        }
    }

    if (status != SPNG_EOI) {
        spng_ctx_free(ctx);
        UI->error("PNG Encoding did not finish successfully");
    }
}

auto png::from_4bpp_tileset_gray(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                                 unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, const long> {
    spng_ctx* ctx {spng_ctx_new(SPNG_CTX_ENCODER)};
    status = spng_set_option(ctx, SPNG_ENCODE_TO_BUFFER, 1);
    CHECK_STATUS;

    spng_ihdr ihdr {.width = tilewidth * TILE_PIXELWIDTH,
                    .height = tileheight * TILE_PIXELWIDTH,
                    .bit_depth = 4,
                    .color_type = SPNG_COLOR_TYPE_GRAYSCALE,
                    .compression_method = 0,
                    .filter_method = 0,
                    .interlace_method = 0};
    status = spng_set_ihdr(ctx, &ihdr);
    CHECK_STATUS;

    std::vector<unsigned char> pngrow(tilewidth * BYTES_PER_TILEWIDTH_4BPP);
    if (inversed) {
        encode_from_8x8_tiles(src, tilewidth, tileheight, ctx, pngrow,
                              [&pngrow](const unsigned column_4bpp, const unsigned char srcbyte) {
                                  pngrow.at(column_4bpp) =
                                      std::rotl(static_cast<unsigned char>(~srcbyte), 4);
                              });

    } else {
        encode_from_8x8_tiles(src, tilewidth, tileheight, ctx, pngrow,
                              [&pngrow](const unsigned byteindex, const unsigned char srcbyte) {
                                  pngrow.at(byteindex) = std::rotl(srcbyte, 4);
                              });
    }

    unsigned long pngsize {};
    auto pngbuffer {std::unique_ptr<const char[]>(
        static_cast<const char*>(spng_get_png_buffer(ctx, &pngsize, &status)))};
    CHECK_STATUS;

    spng_ctx_free(ctx);
    return {std::move(pngbuffer), pngsize};
}

auto png::from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight, const std::vector<u8>& pngpalette)
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
