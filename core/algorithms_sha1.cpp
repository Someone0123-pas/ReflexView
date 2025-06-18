#include <array>
#include <bit>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "algorithms.h"
#include "constants.h"

#define FORMATHASH(hn) std::setw(8) << hn

/*
 * This function is based on https://en.wikipedia.org/wiki/SHA-1#Examples_and_pseudocode,
 * licensed under CC BY-SA 4.0
 * (https://en.wikipedia.org/wiki/Wikipedia:Text_of_the_Creative_Commons_Attribution-ShareAlike_4.0_International_License#License),
 * and was modified for caluclating the sha1sum of a GBA ROM
 */
auto sha1::calculate(const RawView& rom) -> std::string {
    u32 h0 {0x67452301};
    u32 h1 {0xefcdab89};
    u32 h2 {0x98badcfe};
    u32 h3 {0x10325476};
    u32 h4 {0xc3d2e1f0};

    std::array<unsigned char, 0x40> lastchunk {};  // 512 Bits
    lastchunk.at(0) = 0x80;                        // Append 1
    lastchunk.at(0x3C) = 0x08;                     // Append message length in bits

    for (unsigned chunk {0}; chunk <= ROM_SIZE; chunk += 0x40) {
        std::array<u32, 80> w {};

        // Initialise w by reading every 4 bytes as big-endian u32
        if (chunk < ROM_SIZE) {
            for (unsigned i {0}; i < 16; ++i)
                w.at(i) = (rom[chunk + i * 4 + 0] << 24) | (rom[chunk + i * 4 + 1] << 16) |
                          (rom[chunk + i * 4 + 2] << 8) | (rom[chunk + i * 4 + 3]);
        } else {
            for (unsigned i {0}; i < 16; ++i)
                w.at(i) = lastchunk.at(i * 4 + 0) << 24 | lastchunk.at(i * 4 + 1) << 16 |
                          lastchunk.at(i * 4 + 2) << 8 | lastchunk.at(i * 4 + 3);
        }

        for (unsigned i {16}; i < 80; ++i)
            w.at(i) = std::rotl(w.at(i - 3) ^ w.at(i - 8) ^ w.at(i - 14) ^ w.at(i - 16), 1);

        u32 a {h0};
        u32 b {h1};
        u32 c {h2};
        u32 d {h3};
        u32 e {h4};
        u32 f {}, k {}, tmp {};

        for (unsigned i {0}; i < 80; ++i) {
            if (i < 20) {
                f = (b & c) | (~b & d);
                k = 0x5a827999;
            } else if (i < 40) {
                f = b ^ c ^ d;
                k = 0x6ed9eba1;
            } else if (i < 60) {
                f = (b & c) ^ (b & d) ^ (c & d);
                k = 0x8f1bbcdc;
            } else {
                f = b ^ c ^ d;
                k = 0xca62c1d6;
            }

            tmp = (std::rotl(a, 5)) + f + e + k + w[i];
            e = d;
            d = c;
            c = std::rotl(b, 30);
            b = a;
            a = tmp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    std::ostringstream digest {};
    digest << std::hex << std::setfill('0') << FORMATHASH(h0) << FORMATHASH(h1) << FORMATHASH(h2)
           << FORMATHASH(h3) << FORMATHASH(h4);
    return digest.str();
}

#undef FORMATHASH
