#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>

#include "byteviewer.h"
#include "structviewer/tileset.h"
#include "types.h"

class TilemapView : public Byteviewer {
    const logical_offset offset;
    const unsigned size;
    const unsigned palette_startnum;

public:
    TilemapView(logical_offset offset, unsigned size, unsigned palette_startnum)
        : Byteviewer(), offset {offset}, size {size}, palette_startnum {palette_startnum} {}
    auto baseadr() const -> logical_offset override;

    // Corrects palette according to palette_startnum so that it starts from 0
    auto get_tile_palette_shifted(unsigned index) const -> u8;

    // Returns the tiles in TilesetView according to the tilemap, just as the png functions require
    // it
    auto assemble_4bpp_tiled_image(const TilesetView&) const
        -> std::pair<std::unique_ptr<const char[]>, long>;
};

#endif  // TILEMAP_H
