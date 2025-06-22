#ifndef LZSS_H
#define LZSS_H

#include <memory>

#include "byteviewer.h"

namespace lzss {
auto decompress(const RawView&) -> std::pair<std::unique_ptr<const char[]>, const long>;
}

#endif  // LZSS_H
