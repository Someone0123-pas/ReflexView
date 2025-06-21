#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <memory>
#include <string>
#include <vector>

#include "byteviewer.h"

namespace sha1 {
auto calculate(const RawView& = {}) -> std::string;
}

namespace png {
// To byte match a grayscale PNG with YamaArashi's gbagfx tool, the grayscale palette must be
// inversed
auto from_4bpp_tileset_gray(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, const long>;

auto from_4bpp_tileset(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                       unsigned tileheight, const std::vector<u8>& pngpalette)
    -> std::pair<std::unique_ptr<const char[]>, const long>;
}  // namespace png

namespace lzss {
auto decompress(const RawView&) -> std::pair<std::unique_ptr<const char[]>, const long>;
}

// Run-Length
namespace RL {};

#endif  // ALGORITHMS_H
