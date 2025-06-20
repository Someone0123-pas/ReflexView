#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <memory>
#include <string>

#include "byteviewer.h"

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

namespace png {
auto from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                       unsigned tileheight)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long>;
}

// Should only be used as component in Byteviewers that contain RawViews to LZSS-compressed buffers
class LZSS {
    const RawView& compressed;
    std::shared_ptr<const char[]> decompressed;
    unsigned size;

    void decompress();

public:
    LZSS(const RawView&);
    void dump_4bpp(const std::string& filepath);
    void dump_png(const std::string& filepath, unsigned tilewidth, unsigned tileheight);
};

// Run-Length
class RL {};

#endif  // ALGORITHMS_H
