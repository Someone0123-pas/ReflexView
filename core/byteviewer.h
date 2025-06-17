#ifndef BYTEVIEWER_H
#define BYTEVIEWER_H

#include <memory>

#include "constants.h"

// Pseudo-Singleton base class:
// When viewers are instantiated, they all refer to the same rom, but different parts of it
// If the rom is changed, all viewers automatically refer to the correct (changed) rom
class Byteviewer {
    static std::unique_ptr<const char[]> rom;
    static Region region;
    static void load_rom();

protected:
    Byteviewer();

public:
    virtual ~Byteviewer() = default;
    virtual auto lower_bound() const -> logical_offset = 0;
    virtual auto get_u8(logical_offset) const -> u8;
    virtual auto get_u16(logical_offset) const -> u16;
    virtual auto get_u32(logical_offset) const -> u32;
};

// For sections with unknown size and []-indexing like an array
// Default: Whole ROM
struct RawView : public Byteviewer {
    RawView() : Byteviewer() {}
    ~RawView() override = default;

    static const unsigned size;
    auto lower_bound() const -> logical_offset override;
    auto operator[](physical_offset) const -> u8;
};

#endif  // BYTEVIEWER_H
