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

    BackgroundView bg {0x1a};
    bg.dump_tileset_png_gray("test.native.4bpp.png", false);

    return 0;
}
