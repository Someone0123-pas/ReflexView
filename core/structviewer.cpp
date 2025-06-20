#include "structviewer.h"

#include "algorithms.h"
#include "byteviewer.h"

auto TilesetView::lower_bound() const -> logical_offset { return offset; }

const unsigned RoomPropsView::size {0x28};
auto RoomPropsView::lower_bound() const -> logical_offset { return ROM_gROOMPROPS + index * size; }

const unsigned BackgroundView::size {0x20};
auto BackgroundView::lower_bound() const -> logical_offset {
    return get_u32(ROM_gBACKGROUNDS + index * 4);
}

BackgroundView::BackgroundView(unsigned index)
    : Byteviewer(),
      index {index},
      tilesetview {TilesetView {get_u32(lower_bound() + 8)}},
      decompression {LZSS {tilesetview}} {
    if (index > 0x1e) UI->error("Invalid BackgroundView::Index");
    // logical_offset palette_address {get_u32(lower_bound() + 0x10)};
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(lower_bound()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(lower_bound() + 2); }

auto BackgroundView::get_palette_size() const -> unsigned { return get_u16(lower_bound() + 0x16); }

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) {
    decompression.dump_4bpp(filepath);
}

void BackgroundView::dump_tileset_png(const std::string& filepath) {
    decompression.dump_png(filepath, get_width(), get_height());
}
