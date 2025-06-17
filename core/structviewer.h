#ifndef STRUCTVIEWER_H
#define STRUCTVIEWER_H

#include "algorithms.h"
#include "byteviewer.h"

class TilesetView : public RawView {
    logical_offset offset;

public:
    TilesetView(logical_offset offset) : RawView(), offset {offset} {}
    auto lower_bound() const -> logical_offset override;
};

class PaletteView : public Byteviewer {
    logical_offset offset;

public:
    PaletteView(logical_offset offset) : Byteviewer(), offset {offset} {}
    auto lower_bound() const -> logical_offset override;

    // Get palettes with u16-indexing
    auto get_colour_at(unsigned index) const -> u16;
};

// Views RoomProps structs in gRoomProps
class RoomPropsView : public Byteviewer {
    unsigned index;

public:
    RoomPropsView(unsigned index) : Byteviewer(), index {index} {
        if (index > 0x3d6) UI->error("Invalid RoomPropsView::Index");
    }
    static const unsigned size;
    auto lower_bound() const -> logical_offset override;

    // TODO: Get palettedataIdx, etc.
};

// Views dereferenced LevelInfo_1A0 structs in gBackgrounds
class BackgroundView : public Byteviewer {
    unsigned index;
    TilesetView tilesetview;
    LZSS decompression;

public:
    BackgroundView(unsigned index);
    static const unsigned size;
    auto lower_bound() const -> logical_offset override;

    auto get_width() const -> unsigned;
    auto get_palette_size() const -> unsigned;
    auto get_tilesetview() const -> TilesetView;
    void dump_tileset_4bpp(const std::string& filepath);
    void dump_tileset_png(const std::string& filepath);
    // TODO: get_palette() and get_tilemap()
};

#endif  // STRUCTVIEWER_H
