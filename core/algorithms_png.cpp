extern "C" {
#include <spng.h>
}

#include <memory>

#include "algorithms.h"

auto png::from_4bpp(const std::shared_ptr<const char[]>& src, unsigned size, unsigned pixelwidth)
    -> std::pair<std::unique_ptr<const char[]>, unsigned> {}
