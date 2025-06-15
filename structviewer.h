#ifndef STRUCTVIEWER_H
#define STRUCTVIEWER_H

#include "byteviewer.h"
#include "algorithms.h"

class TilesetView : public RawView {
    logical_offset offset;

public:
    TilesetView(logical_offset offset) : RawView(), offset {offset} {}
    virtual logical_offset lower_bound() const override;
};

class PaletteView : public RawView {
    logical_offset offset;

public:
    PaletteView(logical_offset offset) : RawView(), offset {offset} {}
    virtual logical_offset lower_bound() const override;
    u16 at(unsigned index) const; // Get palettes with u16-indexing
};


// Views RoomProps structs in gRoomProps
class RoomPropsView : public Byteviewer {
    unsigned index;

public:
    RoomPropsView(unsigned index) : Byteviewer(), index {index} {
        if (index > 0x3d6) UI->error("Invalid RoomPropsView::Index");
    }
    static const unsigned size;
    virtual logical_offset lower_bound() const override;

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
    virtual logical_offset lower_bound() const override;

    unsigned get_width() const;
    unsigned get_palette_size() const;
    //const std::vector<u16>& get_palette() const;
    TilesetView get_tilesetview() const;
    void dump_tileset_4bpp(const std::string& filepath);
    void dump_tileset_png(const std::string& filepath);
    // const std::vector<u16>& get_tilemap() const;
};

#endif // STRUCTVIEWER_H
