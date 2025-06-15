#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include "ui.h"

#define ROM_SIZE            16 * 1024 * 1024  // 16 MB
#define ROM_MAPADR          0x08000000
#define ROM_gROOMPROPS      0x089331AC
#define ROM_gBACKGROUNDS    0x08D64A24

#define SHA1SUM_U "274b102b6d940f46861a92b4e65f89a51815c12c"

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;

using physical_offset = unsigned;

// Handles rom-addresses with GBA-IO-Mapping
class logical_offset {
    unsigned offset;

public:
    logical_offset(unsigned offset = ROM_MAPADR) : offset {offset} {
        if (offset < ROM_MAPADR || offset > ROM_MAPADR + ROM_SIZE)
            UI->error("Invalid logical address");
    }
    operator unsigned() { return offset; }
    physical_offset get_physical_offset() { return offset - ROM_MAPADR; }
};

enum class Region { US, EU, JP };

#endif  // CONSTANTS_H
