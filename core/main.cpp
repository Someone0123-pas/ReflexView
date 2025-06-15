#include <iostream>
#include <memory>

#include "algorithms.h"
#include "structviewer.h"
#include "ui.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Print and dump from katam(U).gba\nUsage:\n\t./reflexview ROM-FILE" << std::endl;
        return 1;
    }
    CLI::set_ui(argv[1]);

    std::cout << UI->get_rom_filepath() << std::endl;

    BackgroundView bg {0};
    std::cout << std::hex << (unsigned)bg.get_tilesetview()[1] << std::endl;
    bg.dump_tileset_4bpp("test.4bpp.native");

    return 0;
}
