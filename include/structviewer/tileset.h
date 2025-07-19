#ifndef TILESET_H
#define TILESET_H

#include "byteviewer.h"
#include "types.h"

class TilesetView : public RawView {
    const logical_offset offset;
    const u32 numtiles;

public:
    TilesetView(logical_offset offset, unsigned numtiles) : RawView(), offset {offset}, numtiles {numtiles} {}
    auto baseadr() const -> logical_offset override;
    auto get_numtiles() const -> u32;
};

#endif  // TILESET_H
