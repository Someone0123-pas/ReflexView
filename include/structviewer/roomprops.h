#ifndef ROOMPROPS_H
#define ROOMPROPS_H

#include "byteviewer.h"
#include "types.h"
#include "ui.h"

constexpr unsigned ROM_gROOMPROPS {0x089331AC};

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

#endif  // ROOMPROPS_H
