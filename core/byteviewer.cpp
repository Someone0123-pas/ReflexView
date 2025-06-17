
#include "byteviewer.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "algorithms.h"
#include "constants.h"
#include "ui.h"

std::unique_ptr<const char[]> Byteviewer::rom;
Region Byteviewer::region;

Byteviewer::Byteviewer() {
    if (!rom) load_rom();
}

void Byteviewer::load_rom() {
    std::string filepath = UI->get_rom_filepath();
    if (std::filesystem::file_size(filepath) != ROM_SIZE) UI->error("ROM has incorrect size");

    std::ifstream romfile {filepath, std::ios::binary};
    auto romdump {std::make_unique<char[]>(ROM_SIZE)};

    romfile.read(romdump.get(), ROM_SIZE);
    if (romfile.gcount() != ROM_SIZE) UI->error("ROM has incorrect size");

    rom = std::move(romdump);

    std::string sha1sum {sha1::calculate()};
    if (sha1sum == SHA1SUM_U) region = Region::US;
    else UI->error("Sha1 sum doesn't match known ROMs");
}

auto Byteviewer::get_u8(logical_offset offset) const -> u8 {
    return rom[offset.get_physical_offset()];
}

auto Byteviewer::get_u16(logical_offset offset) const -> u16 {
    return get_u8(offset) | get_u8(offset + 1) << 8;
}

auto Byteviewer::get_u32(logical_offset offset) const -> u32 {
    return get_u16(offset) | get_u16(offset + 2) << 16;
}

const unsigned RawView::size {ROM_SIZE};
auto RawView::lower_bound() const -> logical_offset { return ROM_MAPADR; }
auto RawView::operator[](physical_offset offset) const -> u8 {
    return get_u8(lower_bound() + offset);
}
