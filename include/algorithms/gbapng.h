#ifndef GBAPNG_H
#define GBAPNG_H

#include <memory>
#include <vector>

#include "types.h"

namespace png {
// To byte match a grayscale PNG with YamaArashi's gbagfx tool, the grayscale palette must be
// inversed
auto from_4bpp_tiled_image_gray(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                            unsigned tileheight, bool inversed)
    -> std::pair<std::unique_ptr<const char[]>, const long>;

auto from_4bpp_tiled_image(const std::shared_ptr<const char[]>& src, unsigned tilewidth,
                       unsigned tileheight, const std::vector<u8>& pngpalette)
    -> std::pair<std::unique_ptr<const char[]>, const long>;
}  // namespace png

#endif  // GBAPNG_H
