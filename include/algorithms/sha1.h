#ifndef SHA1_H
#define SHA1_H

#include <string>

#include "byteviewer.h"

const std::string SHA1SUM_U {"274b102b6d940f46861a92b4e65f89a51815c12c"};

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

#endif  // SHA1_H
