#include "structviewer.h"

#include <fstream>
#include <vector>

#include "algorithms/gbapng.h"
#include "algorithms/lzss.h"
#include "constants.h"

constexpr auto RED {[](u16 gbapal) -> u8 { return (gbapal & 0x1f) * 0xff / 0x1f; }};
constexpr auto GREEN {[](u16 gbapal) -> u8 { return ((gbapal >> 5) & 0x1f) * 0xff / 0x1f; }};
constexpr auto BLUE {[](u16 gbapal) -> u8 { return ((gbapal >> 10) & 0x1f) * 0xff / 0x1f; }};

auto TilesetView::baseadr() const -> logical_offset { return offset; }

auto PaletteView::baseadr() const -> logical_offset { return offset; }

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

const unsigned RoomPropsView::size {0x28};
auto RoomPropsView::baseadr() const -> logical_offset { return ROM_gROOMPROPS + index * size; }

const unsigned BackgroundView::size {0x20};
auto BackgroundView::baseadr() const -> logical_offset {
    return get_u32(ROM_gBACKGROUNDS + index * 4);
}

auto BackgroundView::get_tilesetview() const -> const TilesetView {
    return TilesetView {get_u32(baseadr() + 8)};
}
auto BackgroundView::get_paletteview() const -> const PaletteView {
    return PaletteView {get_u32(baseadr() + 0x10), get_u16(baseadr() + 0x16),
                        get_u16(baseadr() + 0x14) / 0x10u};
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(baseadr()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(baseadr() + 2); }

void BackgroundView::dump_tileset_4bpp(const std::string& filepath) {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    std::ofstream {filepath, std::ios::binary}.write(tileset.get(), tileset_size);
}

void BackgroundView::dump_png_gray(const std::string& filepath, bool inversed) {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    auto [pngbuffer, pngbuffer_size] {
        png::from_4bpp_tileset_gray(std::move(tileset), get_width(), get_height(), inversed)};
    std::ofstream {filepath, std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}

void BackgroundView::dump_png(const std::string& filepath) {
    // TODO
}
