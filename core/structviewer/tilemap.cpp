#include "structviewer/tilemap.h"

#include "types.h"

auto TilemapView::baseadr() const -> logical_offset { return offset; }

auto TilemapView::get_tile_palette_shifted(unsigned index) const -> u8 {
    return (get_u16(baseadr() + index * 2) >> 12) - palette_startnum;
}
