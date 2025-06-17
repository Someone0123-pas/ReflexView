extern "C" {
#include <spng.h>
}

#include <memory>

#include "algorithms.h"

auto png::from_4bpp(const char file[], unsigned size, unsigned pixelwidth)
    -> std::pair<std::unique_ptr<const char[]>, unsigned> {}
