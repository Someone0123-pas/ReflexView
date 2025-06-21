#include "structviewer.h"

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

BackgroundView::BackgroundView(unsigned index)
    : Byteviewer(),
      index {index},
      tilesetview {TilesetView {get_u32(baseadr() + 8)}},
      decompression {LZSS {tilesetview}} {
    if (index > 0x1e) UI->error("Invalid BackgroundView::Index");
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(baseadr()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(baseadr() + 2); }

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) {
    decompression.dump_4bpp(filepath);
}

void BackgroundView::dump_tileset_png_gray(const std::string& filepath, bool inversed) {
    decompression.dump_png_gray(filepath, get_width(), get_height(), inversed);
}

void BackgroundView::dump_png(const std::string& filepath) {
    decompression.dump_png(
        filepath, get_width(), get_height(),
        PaletteView {get_u32(baseadr() + 0x10), get_u16(baseadr() + 0x16)});
}
