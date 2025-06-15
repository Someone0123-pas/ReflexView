#include "structviewer.h"

#include "algorithms.h"
#include "byteviewer.h"

logical_offset TilesetView::lower_bound() const { return offset; }

const unsigned RoomPropsView::size {0x28};
logical_offset RoomPropsView::lower_bound() const { return ROM_gROOMPROPS + index * size; }

const unsigned BackgroundView::size {0x20};
logical_offset BackgroundView::lower_bound() const { return get_u32(ROM_gBACKGROUNDS + index * 4); }

BackgroundView::BackgroundView(unsigned index)
    : Byteviewer(),
      index {index},
      tilesetview {TilesetView {get_u32(lower_bound() + 8)}},
      decompression {LZSS {tilesetview}} {
    if (index > 0x1e) UI->error("Invalid BackgroundView::Index");
    // logical_offset palette_address {get_u32(lower_bound() + 0x10)};
}

unsigned BackgroundView::get_width() const { return get_u16(lower_bound()); }

unsigned BackgroundView::get_palette_size() const { return get_u16(lower_bound() + 0x16); }

TilesetView BackgroundView::get_tilesetview() const {
    return tilesetview;
}

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) {
    decompression.dump_4bpp(filepath);
}

void BackgroundView::dump_tileset_png(const std::string& filepath) {
    decompression.dump_png(filepath, get_width());
}
