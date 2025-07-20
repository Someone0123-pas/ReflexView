#include "structviewer/palette.h"

#include <vector>

#include "types.h"

auto PaletteView::baseadr() const -> logical_offset { return offset; }
auto PaletteView::get_startnum() const -> unsigned { return startnum; }

auto PaletteView::get_agbpal() const -> const std::vector<u8> {
    std::vector<u8> agbpal(size * 2);
    for (unsigned index {}; index < size * 2; ++index) { agbpal.at(index) = get_u8(baseadr() + index); }
    return agbpal;
}

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

auto PaletteView::pal_to_c() const -> const std::string {
    std::string result {"const u16 RENAMEPALETTE[] = {"};

    for (unsigned colour_index {}; colour_index < size; ++colour_index) {
        if (colour_index == 0) {
            result += "\n    ";
        }
        else if (colour_index % 8 == 0) {
            result += ",\n    ";
        }
        else {
            result += ", ";
        }

        u16 curcolour {get_u16(baseadr() + colour_index * 2)};
        result += std::format(
            "RGB({:2}, {:2}, {:2}) | {:#06x}",
            curcolour & 0x1f,
            (curcolour >> 5) & 0x1f,
            (curcolour >> 10) & 0x1f,
            curcolour & 0x8000
        );
    }

    result += "\n};\n";
    return result;
}
