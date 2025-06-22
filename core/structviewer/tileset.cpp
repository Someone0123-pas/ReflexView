#include "structviewer/tileset.h"

#include "types.h"

auto TilesetView::baseadr() const -> logical_offset { return offset; }
