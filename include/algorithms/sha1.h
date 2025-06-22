#ifndef SHA1_H
#define SHA1_H

#include "byteviewer.h"

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

#endif  // SHA1_H
