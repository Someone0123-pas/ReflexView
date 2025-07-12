#include "structviewer/palette.h"

#include <vector>

#include "types.h"

auto PaletteView::baseadr() const -> logical_offset { return offset; }

auto PaletteView::get_agbpal() const -> const std::vector<u8> {
    std::vector<u8> agbpal(size * 2);
    for (unsigned index {}; index < size * 2; ++index) { agbpal.at(index) = get_u8(baseadr() + index); }
    return agbpal;
}

// Returns a u8 vector that confirms to the specifications of a struct spng_plte_entry array as
// specified in spng.h
auto PaletteView::get_spng_plte() const -> const std::vector<u8> {
    std::vector<u8> spng_palette(size * 4);

    for (unsigned colour_index {}; colour_index < size; ++colour_index) {
        u16 curcolour {get_u16(baseadr() + colour_index * 2)};
        spng_palette.at(colour_index * 4 + 0) = RED(curcolour);
        spng_palette.at(colour_index * 4 + 1) = GREEN(curcolour);
        spng_palette.at(colour_index * 4 + 2) = BLUE(curcolour);
    }

    return spng_palette;
}
