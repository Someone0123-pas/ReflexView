#include "structviewer/roomprops.h"

#include "types.h"

const unsigned RoomPropsView::size {0x28};
auto RoomPropsView::baseadr() const -> logical_offset { return ROM_gROOMPROPS + index * size; }
