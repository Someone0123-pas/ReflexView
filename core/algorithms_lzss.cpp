#include <memory>
#include <string>
#include <utility>

#include "algorithms.h"
#include "byteviewer.h"
#include "ui.h"

// This decompression implementation is based on the GBATEK reference
// https://problemkaputt.de/gbatek.htm#lzdecompressionfunctions
// and specialised for LZSS only
auto lzss::decompress(const RawView& compressed)
    -> std::pair<std::unique_ptr<const char[]>, const long> {
    if (compressed[0] != 0x10) {
        UI->error("Provided TilesetView is not LZSS compressed (-> doesn't start with 0x10)");
    }
    unsigned buf_size {
        static_cast<unsigned>(compressed[1] | compressed[2] << 0x8 | compressed[3] << 0x10)};
    auto buf {std::make_unique<char[]>(buf_size)};
    unsigned buf_index {0};

    unsigned curindex {4};
    u8 flagbyte {0x00};
    int flagnum {-1};

    while (buf_index < buf_size) {
        if (flagnum < 0) {
            flagbyte = compressed[curindex++];
            flagnum = 7;
        }

        if (flagbyte & (1 << flagnum)) {  // Compressed Byte
            unsigned len {static_cast<unsigned>(compressed[curindex] >> 0x4) + 3 + buf_index};
            unsigned disp {static_cast<unsigned>(compressed[curindex++] & 0xf) << 0x8};
            disp += compressed[curindex++] + 1;

            for (; buf_index < len; ++buf_index) { buf[buf_index] = buf[buf_index - disp]; }
        } else {  // Data Byte
            buf[buf_index++] = static_cast<char>(compressed[curindex++]);
        }

        --flagnum;
    }

    return {std::move(buf), buf_size};
}
