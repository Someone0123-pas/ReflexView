#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <memory>
#include <string>

#include "byteviewer.h"
#include "constants.h"

namespace sha1 {
std::string calculate(RawView = {});
}

namespace png {
std::pair<std::unique_ptr<const char[]>, unsigned> from_4bpp(const char buffer[], unsigned size,
                                                             unsigned width);
}

class LZSS {
    const RawView& tiles;
    std::unique_ptr<const char[]> decompressed;
    unsigned size;

    void decompress();

public:
    LZSS(const RawView&);
    void dump_4bpp(const std::string& filepath);
    void dump_png(const std::string& filepath, unsigned width);
};

// Run-Length
class RL {};

#endif  // ALGORITHMS_H
