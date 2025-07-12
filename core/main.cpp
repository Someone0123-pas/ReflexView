#include "ui.h"

auto main(int argc, char* argv[]) -> int {
    // TODO: There surely must be better argparser libraries out there
    switch (argc) {
        case 1:
            QT::set_ui();
            break;
        case 2:
            QT::set_ui(argv[1]);
            break;
        default:
            CLI::set_ui(argv[1]);
            break;
    }

    return UI->run(argc, argv);
}
