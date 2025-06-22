#ifndef TILESET_H
#define TILESET_H

#include "byteviewer.h"
#include "types.h"

class TilesetView : public RawView {
    const logical_offset offset;

public:
    TilesetView(logical_offset offset) : RawView(), offset {offset} {}
    auto baseadr() const -> logical_offset override;
};

#endif  // TILESET_H
