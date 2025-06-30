#include "structviewer/background.h"

#include <fstream>
#include <string>

#include "algorithms/gbapng.h"
#include "algorithms/lzss.h"
#include "structviewer/palette.h"
#include "structviewer/tilemap.h"
#include "structviewer/tileset.h"
#include "types.h"

const unsigned BackgroundView::size {0x20};
const unsigned BackgroundView::max_index {0x1e};
auto BackgroundView::baseadr() const -> logical_offset { return get_u32(ROM_gBACKGROUNDS + index * 4); }

auto BackgroundView::get_tilesetview() const -> const TilesetView {
    return TilesetView {get_u32(baseadr() + 8)};
}
auto BackgroundView::get_paletteview() const -> const PaletteView {
    return PaletteView {
        get_u32(baseadr() + 0x10), get_u16(baseadr() + 0x16), get_u16(baseadr() + 0x14) / 0x10u
    };
}

auto BackgroundView::get_tilemapview() const -> const TilemapView {
    return TilemapView {
        get_u32(baseadr() + 0x18), get_u32(baseadr() + 0xc), get_u16(baseadr() + 0x14) / 0x10u
    };
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(baseadr()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(baseadr() + 2); }

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) const {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    std::ofstream {filepath, std::ios::binary}.write(tileset.get(), tileset_size);
}

void BackgroundView::dump_png_gray(const std::string& filepath, bool inversed) const {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    auto [pngbuffer, pngbuffer_size] {
        png::from_4bpp_tiled_image_gray(tileset.get(), get_width(), get_height(), inversed)
    };
    std::ofstream {filepath, std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}

void BackgroundView::dump_png(const std::string& filepath) const {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    auto [pngbuffer, pngbuffer_size] {png::from_4bpp_tiled_image(
        tileset.get(), get_width(), get_height(), get_paletteview(), get_tilemapview()
    )};
    std::ofstream {filepath, std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}
