#include "structviewer/background.h"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "algorithms/gbapng.h"
#include "algorithms/lzss.h"
#include "structviewer/palette.h"
#include "structviewer/tilemap.h"
#include "structviewer/tileset.h"
#include "types.h"

const unsigned BackgroundView::size {0x20};
const unsigned BackgroundView::max_index {0x23};
auto BackgroundView::baseadr() const -> logical_offset { return get_u32(ROM_gBACKGROUNDS + index * 4); }

auto BackgroundView::get_tilesetview() const -> const TilesetView {
    return TilesetView {get_u32(baseadr() + 8), get_u32(baseadr() + 12)};
}
auto BackgroundView::get_paletteview() const -> const PaletteView {
    return PaletteView {
        get_u32(baseadr() + 0x10), get_u16(baseadr() + 0x16), get_u16(baseadr() + 0x14) / 0x10u
    };
}

auto BackgroundView::get_tilemapview() const -> const TilemapView {
    return TilemapView {
        get_u32(baseadr() + 0x18), get_width() * get_height(), get_u16(baseadr() + 0x14) / 0x10u
    };
}

auto BackgroundView::get_width() const -> unsigned { return get_u16(baseadr()); }
auto BackgroundView::get_height() const -> unsigned { return get_u16(baseadr() + 2); }

auto BackgroundView::get_png_assembled() const -> std::pair<std::unique_ptr<const char[]>, const long> {
    // The tilesets of backgrounds 0-32 are already assembled as tiled_image as-is, so they can just be passed
    // as such
    if (index <= 32) {
        return png::from_4bpp_tiled_image(
            lzss::decompress(get_tilesetview()).first.get(), get_width(), get_height(), get_paletteview(),
            get_tilemapview()
        );
    }
    return png::from_4bpp_tiled_image(
        get_tilemapview().assemble_4bpp_tiled_image(get_tilesetview()).data(), get_width(), get_height(),
        get_paletteview(), get_tilemapview()
    );
}

auto BackgroundView::get_tileset_png() const -> std::pair<std::unique_ptr<const char[]>, const long> {
    if (index <= 31) {
        return png::from_4bpp_tiled_image(
            lzss::decompress(get_tilesetview()).first.get(), get_width(), get_height(), get_paletteview(),
            get_tilemapview()
        );
    }
    return png::from_tileset(get_tilesetview(), get_width(), get_paletteview(), get_tilemapview());
}

auto BackgroundView::get_tileset_png_gray(bool inversed) const
    -> std::pair<std::unique_ptr<const char[]>, const long> {
    return png::from_4bpp_tiled_image_gray(
        lzss::decompress(get_tilesetview()).first.get(), get_width(), get_height(), inversed
    );
}

void BackgroundView::dump_tileset_4bpp(const std::string& filename) const {
    auto [tileset, tileset_size] {lzss::decompress(get_tilesetview())};
    std::ofstream {filename + ".4bpp", std::ios::binary}.write(tileset.get(), tileset_size);
}

void BackgroundView::dump_all(const std::string& filename) const {
    auto [pngbuffer, pngbuffer_size] {get_tileset_png()};
    std::ofstream {filename + ".png", std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);

    const std::vector<u8>& tilemap {get_tilemapview().get_tilemap()};
    const std::vector<char>& tilemap_char {tilemap.begin(), tilemap.end()};
    std::ofstream {filename + ".tilemap", std::ios::binary}.write(
        tilemap_char.data(), static_cast<long>(tilemap_char.size())
    );

    const std::vector<u8>& palette {get_paletteview().get_agbpal()};
    const std::vector<char>& palette_char {palette.begin(), palette.end()};
    std::ofstream {filename + ".agbpal", std::ios::binary}.write(
        palette_char.data(), static_cast<long>(palette_char.size())
    );
}

void BackgroundView::dump_gray(const std::string& filename, bool inversed) const {
    auto [pngbuffer, pngbuffer_size] {get_tileset_png_gray(inversed)};
    std::ofstream {filename + ".png", std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}

void BackgroundView::dump_assembled(const std::string& filename) const {
    auto [pngbuffer, pngbuffer_size] {get_png_assembled()};
    std::ofstream {filename + ".png", std::ios::binary}.write(pngbuffer.get(), pngbuffer_size);
}
