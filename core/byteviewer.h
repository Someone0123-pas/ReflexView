#ifndef BYTEVIEWER_H
#define BYTEVIEWER_H

#include <memory>
#include <string>
#include <vector>

#include "constants.h"
#include "ui.h"

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
    virtual logical_offset lower_bound() const = 0;
    virtual u8 get_u8(logical_offset) const;
    virtual u16 get_u16(logical_offset) const;
    virtual u32 get_u32(logical_offset) const;
};

// For sections with unknown size and []-indexing like an array
// Default: Whole ROM
struct RawView : public Byteviewer {
    RawView() : Byteviewer() {}
    virtual ~RawView() = default;
    static const unsigned size;
    virtual logical_offset lower_bound() const override;
    u8 operator[](physical_offset) const;
};

#endif  // BYTEVIEWER_H
