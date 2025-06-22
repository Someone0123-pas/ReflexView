#ifndef PALETTE_H
#define PALETTE_H

#include <vector>

#include "byteviewer.h"
#include "types.h"

constexpr auto RED {[](u16 gbapal) -> u8 { return (gbapal & 0x1f) * 0xff / 0x1f; }};
constexpr auto GREEN {[](u16 gbapal) -> u8 { return ((gbapal >> 5) & 0x1f) * 0xff / 0x1f; }};
constexpr auto BLUE {[](u16 gbapal) -> u8 { return ((gbapal >> 10) & 0x1f) * 0xff / 0x1f; }};

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

#endif  // PALETTE_H
