#include "structviewer/tilemap.h"

#include <cassert>

#include "types.h"
#include "ui.h"

auto TilemapView::baseadr() const -> logical_offset { return offset; }

auto TilemapView::get_tilemap() const -> const std::vector<u8> {
    std::vector<u8> bin(size * 2);
    for (unsigned index {}; index < size * 2; ++index) { bin.at(index) = get_u8(baseadr() + index); }
    return bin;
}

auto TilemapView::get_tile_palette_shifted(unsigned index) const -> u8 {
    if (index > size) {
        UI->error("Tilemap index too big");
    }
    unsigned palette_unshifted {static_cast<unsigned>(get_u16(baseadr() + index * 2) >> 12)};
    if (palette_unshifted < palette_startnum) {
        UI->error(
            "The tilemap references palettes not specified in the same struct. The palette is thus decided "
            "at runtime, which has not yet been implemented."
        );
    }
    return palette_unshifted - palette_startnum;
}
