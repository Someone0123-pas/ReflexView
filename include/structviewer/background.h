#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <filesystem>
#include <string>

#include "byteviewer.h"
#include "structviewer/palette.h"
#include "structviewer/tilemap.h"
#include "structviewer/tileset.h"
#include "types.h"

constexpr unsigned ROM_gBACKGROUNDS {0x08D64A24};

// Views dereferenced LevelInfo_1A0 structs in gBackgrounds
class BackgroundView : public Byteviewer {
    const unsigned index;

    auto get_tilesetview() const -> const TilesetView;
    auto get_paletteview() const -> const PaletteView;
    auto get_tilemapview() const -> const TilemapView;

public:
    BackgroundView(unsigned index) : Byteviewer(), index {index} {
        if (index > max_index) {
            UI->error("Internal Error: Indices", "An invalid BackgroundView::index was given");
        }
    }
    static const unsigned size;
    static const unsigned max_index;
    auto baseadr() const -> logical_offset override;

    auto get_width() const -> unsigned;
    auto get_height() const -> unsigned;

    auto get_png_assembled() const -> std::pair<std::unique_ptr<const char[]>, const long>;
    auto get_tileset_png() const -> std::pair<std::unique_ptr<const char[]>, const long>;
    auto get_tileset_png_gray(bool inversed) const -> std::pair<std::unique_ptr<const char[]>, const long>;

    // Dump tileset as .4bpp
    void dump_tileset_4bpp(const std::string& filename) const;

    /*
     * Dump tileset as .png, Tilemap as .tilemap and Palette as .agbpal.
     * The assembled background is equivalent to the tileset for most backgrounds (except the credits).
     * Filenames always without extension.
     */
    void dump_all(const std::filesystem::path& dirname) const;

    void dump_gray(const std::string& filename, bool inversed) const;
    void dump_agbpal(const std::string& filename) const;
    void dump_assembled(const std::string& filename) const;
};

#endif  // BACKGROUND_H
