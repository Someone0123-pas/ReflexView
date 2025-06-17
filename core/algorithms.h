#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <memory>
#include <string>

#include "byteviewer.h"

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

namespace png {
auto from_4bpp(const char buffer[], unsigned size, unsigned pixelwidth)
    -> std::pair<std::unique_ptr<const char[]>, unsigned>;
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
