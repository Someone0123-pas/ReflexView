#ifndef STRUCTVIEWER_H
#define STRUCTVIEWER_H

#include <vector>

#include "byteviewer.h"
#include "constants.h"

class TilesetView : public RawView {
    const logical_offset offset;

public:
    TilesetView(logical_offset offset) : RawView(), offset {offset} {}
    auto baseadr() const -> logical_offset override;
};

class PaletteView : public Byteviewer {
    const logical_offset offset;
    const unsigned size;
    const unsigned startnum;  // Starting index (0 - 15), where palette is loaded into

public:
    PaletteView(logical_offset offset, unsigned size, unsigned startnum)
        : Byteviewer(), offset {offset}, size {size}, startnum {startnum} {}
    auto baseadr() const -> logical_offset override;

    auto get_spng_plte() const -> const std::vector<u8>;
};

// Views RoomProps structs in gRoomProps
class RoomPropsView : public Byteviewer {
    const unsigned index;

public:
    RoomPropsView(unsigned index) : Byteviewer(), index {index} {
        if (index > 0x3d6) UI->error("Invalid RoomPropsView::index");
    }
    static const unsigned size;
    auto baseadr() const -> logical_offset override;

    // TODO: Get palettedataIdx, etc.
};

// Views dereferenced LevelInfo_1A0 structs in gBackgrounds
class BackgroundView : public Byteviewer {
    const unsigned index;

    auto get_tilesetview() const -> const TilesetView;
    auto get_paletteview() const -> const PaletteView;

public:
    BackgroundView(unsigned index) : Byteviewer(), index {index} {
        if (index > 0x1e) { UI->error("Invalid BackgroundView::index"); }
    }
    static const unsigned size;
    auto baseadr() const -> logical_offset override;

    auto get_width() const -> unsigned;
    auto get_height() const -> unsigned;
    void dump_tileset_4bpp(const std::string& filepath);
    void dump_png_gray(const std::string& filepath, bool inversed);
    void dump_png(const std::string& filepath);
};

#endif  // STRUCTVIEWER_H
