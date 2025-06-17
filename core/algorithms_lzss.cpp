#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "algorithms.h"
#include "byteviewer.h"
#include "ui.h"

LZSS::LZSS(const RawView& tiles) : tiles {tiles} {}

// This decompression implementation is based on the GBATEK reference
// https://problemkaputt.de/gbatek.htm#lzdecompressionfunctions
// and specialised for LZSS only
void LZSS::decompress() {
    if (tiles[0] != 0x10) {
        UI->error("Provided TilesetView is not LZSS compressed (-> doesn't start with 0x10)");
    }
    unsigned buffersize {static_cast<unsigned>(tiles[1] | tiles[2] << 0x8 | tiles[3] << 0x10)};
    auto decomp_tiles {std::make_unique<char[]>(buffersize)};
    unsigned decomp_tiles_index {0};

    unsigned curindex {4};
    u8 flagbyte {0x00};
    int flagnum {-1};

    while (decomp_tiles_index < buffersize) {
        if (flagnum < 0) {
            flagbyte = tiles[curindex++];
            flagnum = 7;
        }

        if (flagbyte & (1 << flagnum)) {  // Compressed Byte
            unsigned len {static_cast<unsigned>(tiles[curindex] >> 0x4) + 3 + decomp_tiles_index};
            unsigned disp {static_cast<unsigned>(tiles[curindex++] & 0xf) << 0x8};
            disp += tiles[curindex++] + 1;

            for (; decomp_tiles_index < len; ++decomp_tiles_index) {
                decomp_tiles[decomp_tiles_index] = decomp_tiles[decomp_tiles_index - disp];
            }
        } else {  // Data Byte
            decomp_tiles[decomp_tiles_index++] = static_cast<char>(tiles[curindex++]);
        }

        --flagnum;
    }

    decompressed = std::move(decomp_tiles);
    size = buffersize;
}

void LZSS::dump_4bpp(const std::string& filepath) {
    if (!decompressed) decompress();
    std::ofstream outputfile {filepath, std::ios::binary};
    outputfile.write(decompressed.get(), size);
}

void LZSS::dump_png(const std::string& filepath, unsigned width) {
    if (!decompressed) decompress();
    std::ofstream outputfile {filepath, std::ios::binary};
    auto [pngbuffer, pngsize] {png::from_4bpp(decompressed.get(), size, width * 8)};
    outputfile.write(pngbuffer.get(), pngsize);
}
