#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <memory>
#include <string>

#include "byteviewer.h"
class PaletteView;

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

namespace png {
// To byte match a grayscale PNG with YamaArashi's gbagfx tool, the grayscale palette must be
// inversed
auto from_4bpp_tileset_gray(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long>;

auto from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                       unsigned tileheight, const PaletteView& palette)
    -> std::pair<std::unique_ptr<const char[]>, unsigned long>;
}  // namespace png

// Should only be used as component in Byteviewers that contain RawViews to LZSS-compressed buffers
class LZSS {
    const RawView& compressed;
    std::shared_ptr<const char[]> decompressed;
    unsigned size;

    void decompress();

public:
    LZSS(const RawView&);
    void dump_4bpp(const std::string& filepath);
    void dump_png_gray(const std::string& filepath, unsigned tilewidth, unsigned tileheight,
                       bool inversed);
    void dump_png(const std::string& filepath, unsigned tilewidth, unsigned tileheight, const PaletteView& palette);
};

// Run-Length
class RL {};

#endif  // ALGORITHMS_H
