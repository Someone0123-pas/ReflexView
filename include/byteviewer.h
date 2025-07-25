#ifndef BYTEVIEWER_H
#define BYTEVIEWER_H

#include <memory>

#include "types.h"

// Pseudo-Singleton base class:
// When viewers are instantiated, they all refer to the same rom, but different parts of it
// If the rom is changed, all viewers automatically refer to the correct (changed) rom
class Byteviewer {
    static std::unique_ptr<const char[]> rom;
    static Region region;

protected:
    Byteviewer();

public:
    virtual ~Byteviewer() = default;
    static void load_rom();

    virtual auto baseadr() const -> logical_offset = 0;
    virtual auto get_u8(const logical_offset&) const -> u8;
    virtual auto get_u16(const logical_offset&) const -> u16;
    virtual auto get_u32(const logical_offset&) const -> u32;
};

// For sections with unknown size and []-indexing like an array
// Default: Whole ROM
struct RawView : public Byteviewer {
    RawView() : Byteviewer() {}
    ~RawView() override = default;

    static const unsigned size;
    auto baseadr() const -> logical_offset override;
    auto operator[](physical_offset) const -> u8;
};

#endif  // BYTEVIEWER_H
