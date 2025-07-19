#ifndef GBAPNG_H
#define GBAPNG_H

#include <memory>

#include "structviewer/palette.h"
#include "structviewer/tilemap.h"
#include "structviewer/tileset.h"

static constexpr unsigned BITDEPTH_4BPP {4};  // That's why it's called 4bpp, duh
static constexpr unsigned TILE_PIXELWIDTH {8};
static constexpr unsigned TILE_PIXELHEIGHT {8};
static constexpr unsigned TILE_BYTEWIDTH_4BPP {BITDEPTH_4BPP * TILE_PIXELWIDTH / 8};
static constexpr unsigned TILE_BYTES_4BPP {TILE_BYTEWIDTH_4BPP * TILE_PIXELHEIGHT};

// PNG functions expect a tiled_image as first parametre, i.e. a byte sequence as if it was a
// 4bpp/8bpp tileset loaded into GBA VRAM. This is to make the png encoding of backgrounds (where
// the tileset happens to be precisely assembled as how the image looks) as convenient as possible.
namespace png {
// To byte match a grayscale PNG with YamaArashi's gbagfx tool, the grayscale palette must be
// inversed
auto from_4bpp_tiled_image_gray(
    const char tiled_image[], unsigned tilewidth, unsigned tileheight, bool inversed
) -> std::pair<std::unique_ptr<const char[]>, const long>;

auto from_4bpp_tiled_image(const char tiled_image[], unsigned tilewidth, unsigned tileheight, const PaletteView&, const TilemapView&)
    -> std::pair<std::unique_ptr<const char[]>, const long>;

auto from_tileset(const TilesetView&, unsigned tilewidth, const PaletteView&, const TilemapView&)
    -> std::pair<std::unique_ptr<const char[]>, const long>;

}  // namespace png

#endif  // GBAPNG_H
