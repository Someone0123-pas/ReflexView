#include "structviewer/tileset.h"

#include "types.h"

auto TilesetView::baseadr() const -> logical_offset { return offset; }
auto TilesetView::get_numtiles() const -> u32 { return numtiles; }
