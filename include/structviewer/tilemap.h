#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>

#include "byteviewer.h"
#include "structviewer/tileset.h"
#include "types.h"

class TilemapView : public Byteviewer {
    const logical_offset offset;
    const unsigned numentries;
    const unsigned palette_startnum;

public:
    TilemapView(logical_offset offset, unsigned numentries, unsigned palette_startnum)
        : Byteviewer(), offset {offset}, numentries {numentries}, palette_startnum {palette_startnum} {}
    auto baseadr() const -> logical_offset override;

    auto get_tilemap() const -> const std::vector<u8>;

    /*
     * Assembles tileset into a tiled_image, i.e. duplicates and rotates tiles according to the tilemap
     * entries
     */
    auto assemble_4bpp_tiled_image(const TilesetView&) const -> const std::vector<char>;

    /*
     * Corrects palette according to palette_startnum so that it starts from 0.
     * This only works correctly, if the tileset has already been assembled to a tiled_image
     */
    auto get_tile_palette_shifted(unsigned mapentryindex) const -> u8;

    // TODO: Implement
    /*
     * Returns the palette for this tile that was first found among the mapentries
     */
    // auto find_palette(unsigned tileindex) const -> u8;
};

#endif  // TILEMAP_H
