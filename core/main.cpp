#include <iostream>

#include "structviewer/background.h"
#include "ui.h"

auto main(int argc, char* argv[]) -> int {
    if (argc != 2) {
        std::cout << "Print and dump from katam(U).gba\nUsage:\n\t./reflexview ROM-FILE\n";
        return 1;
    }
    CLI::set_ui(argv[1]);

    BackgroundView bg {0x0};
    bg.dump_png_gray("test.gray.png", true);
    bg.dump_png("test.colour.png");

    return 0;
}
