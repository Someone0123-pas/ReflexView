#include "structviewer.h"

#include <fstream>

#include "algorithms.h"
#include "byteviewer.h"

auto TilesetView::baseadr() const -> logical_offset { return offset; }

auto PaletteView::baseadr() const -> logical_offset { return offset; }

const unsigned RoomPropsView::size {0x28};
auto RoomPropsView::baseadr() const -> logical_offset { return ROM_gROOMPROPS + index * size; }

const unsigned BackgroundView::size {0x20};
auto BackgroundView::baseadr() const -> logical_offset {
    return get_u32(ROM_gBACKGROUNDS + index * 4);
}

auto BackgroundView::get_tilesetview() const -> const TilesetView {
    return TilesetView {get_u32(baseadr() + 8)};
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(baseadr()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(baseadr() + 2); }

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    std::ofstream {filepath, std::ios::binary}.write(tileset.get(), tileset_size);
}

void BackgroundView::dump_png_gray(const std::string& filepath, bool inversed) {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    auto [pngbuffer, pngbuffer_size] {
        png::from_4bpp_tileset_gray(std::move(tileset), get_width(), get_height(), inversed)};
    std::ofstream {filepath, std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}

void BackgroundView::dump_png(const std::string& filepath) {
    // TODO
}
