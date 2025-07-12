#include <filesystem>
#include <string>

#include "qt/init.h"
#include "ui.h"

QT::QT(const std::string& filepath) : UserInterface(filepath) {}

void QT::set_ui() {
    if (UI) {
        UI->error("UI has already been set");
    }
    UI = std::unique_ptr<QT>(new QT {});
}

void QT::set_ui(const std::string& filepath) {
    if (UI) {
        UI->error("UI has already been set");
    }
    UI = std::unique_ptr<QT>(new QT {filepath});

    if (!std::filesystem::exists(filepath)) {
        UI->error("File doesn't exist");
    }
}

auto QT::get_rom_filepath() const -> std::string {
    // TODO: Check whether filepath is empty, then ask for ROM
    if (!std::filesystem::exists(filepath)) {
        error("File doesn't exist");
    }
    return filepath;
}

void QT::set_rom_filepath(const std::string& filepath) {
    UserInterface::filepath = filepath;
}

void QT::error(const std::string& errormessage) const {
    // TODO
}

auto QT::run(int argc, char* argv[]) -> int { return window_initialise(argc, argv); }
