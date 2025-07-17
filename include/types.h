#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#include "ui.h"

constexpr unsigned ROM_SIZE {16 * 1024 * 1024};  // 16 MB
constexpr unsigned ROM_MAPADR {0x08000000};

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;

using physical_offset = unsigned;

// Handles rom-addresses with GBA-IO-Mapping
class logical_offset {
    const unsigned offset;

public:
    logical_offset(unsigned offset = ROM_MAPADR) : offset {offset} {
        if (offset < ROM_MAPADR || offset > ROM_MAPADR + ROM_SIZE)
            UI->error("Internal Error: Logical Address", "An invalid logical address was given.");
    }
    operator unsigned() const { return offset; }
    auto get_physical_offset() const -> physical_offset { return offset - ROM_MAPADR; }
};

enum class Region : char { US, EU, JP };

#endif  // CONSTANTS_H
