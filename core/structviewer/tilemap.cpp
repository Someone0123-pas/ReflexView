#include "structviewer/tilemap.h"

#include <cassert>
#include <cstring>
#include <vector>

#include "algorithms/gbapng.h"
#include "algorithms/lzss.h"
#include "structviewer/tileset.h"
#include "types.h"
#include "ui.h"

static constexpr auto get_tileindex {[](const u16& mapentry) -> u16 { return mapentry & 0x3ff; }};
static constexpr auto is_flipped_horizontally {[](const u16& mapentry) -> bool { return mapentry & 0x400; }};
static constexpr auto is_flipped_vertically {[](const u16& mapentry) -> bool { return mapentry & 0x800; }};

auto TilemapView::baseadr() const -> logical_offset { return offset; }

auto TilemapView::get_tilemap() const -> const std::vector<u8> {
    std::vector<u8> bin(numentries * 2);
    for (unsigned index {}; index < numentries * 2; ++index) { bin.at(index) = get_u8(baseadr() + index); }
    return bin;
}

auto TilemapView::assemble_4bpp_tiled_image(const TilesetView& tilesetview) const -> const std::vector<char> {
    std::vector<char> tiled_image(numentries * TILE_BYTES_4BPP);
    auto [tileset, tileset_size] {lzss::decompress(tilesetview)};

    for (unsigned mapentryindex {}; mapentryindex < numentries; ++mapentryindex) {
        const u16& mapentry {get_u16(baseadr() + mapentryindex * 2)};
        for (unsigned tilebyteindex {}; tilebyteindex < TILE_BYTES_4BPP; ++tilebyteindex) {
            tiled_image.at(tilebyteindex + mapentryindex * TILE_BYTES_4BPP) =
                static_cast<char>(tileset[tilebyteindex + get_tileindex(mapentry) * TILE_BYTES_4BPP]);
        }

        if (is_flipped_horizontally(mapentry)) {
            UI->error(
                "Not implemented yet: Horizontal flip", std::format(
                                                            "Tilemapentry {} has the horizontal-flip flag "
                                                            "set, but this has not yet been implemented.",
                                                            mapentryindex
                                                        )
            );
        }

        if (is_flipped_vertically(mapentry)) {
            UI->error(
                "Not implemented yet: Vertical flip",
                std::format(
                    "Tilemapentry {} has the vertical-flip flag set, but this has not yet been implemented.",
                    mapentryindex
                )
            );
        }
    }

    return tiled_image;
}

auto TilemapView::get_tile_palette_shifted(unsigned mapentryindex) const -> u8 {
    if (mapentryindex > numentries) {
        UI->error("Internal Error: Indices", "The given tileindex exceeds the size of the tilemap.");
    }
    unsigned palette_unshifted {static_cast<unsigned>(get_u16(baseadr() + mapentryindex * 2) >> 12)};
    if (palette_unshifted < palette_startnum) {
        UI->error(
            "Not implemented yet: Runtime palettes",
            "The tilemap references palettes not specified in the same struct. The palette is thus decided "
            "at runtime, which has not yet been implemented."
        );
    }
    return palette_unshifted - palette_startnum;
}
