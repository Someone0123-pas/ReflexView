#include <iostream>
#include <memory>

#include "structviewer.h"
#include "ui.h"

auto main(int argc, char* argv[]) -> int {
    if (argc != 2) {
        std::cout << "Print and dump from katam(U).gba\nUsage:\n\t./reflexview ROM-FILE\n";
        return 1;
    }
    CLI::set_ui(argv[1]);

    std::cout << UI->get_rom_filepath() << '\n';

    BackgroundView bg {0};
    std::cout << std::hex << (unsigned)bg.get_tilesetview()[1] << '\n';
    bg.dump_tileset_4bpp("test.4bpp.native");

    return 0;
}
