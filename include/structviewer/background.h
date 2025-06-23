#ifndef BACKGROUND_H
#define BACKGROUND_H

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
        if (index > 0x1e) {
            UI->error("Invalid BackgroundView::index");
        }
    }
    static const unsigned size;
    auto baseadr() const -> logical_offset override;

    auto get_width() const -> unsigned;
    auto get_height() const -> unsigned;
    void dump_tileset_4bpp(const std::string& filepath);
    void dump_png_gray(const std::string& filepath, bool inversed);
    void dump_png(const std::string& filepath);
};

#endif  // BACKGROUND_H
