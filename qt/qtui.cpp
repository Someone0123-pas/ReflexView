#include "qt/qtui.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <print>
#include <stdexcept>
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

    if (!std::filesystem::exists(filepath)) {
        UI = std::unique_ptr<QT>(new QT);
        try {
            UI->error("File doesn't exist");
        } catch (std::runtime_error& e) { std::println(std::cerr, "Error: {}", e.what()); }
    } else {
        UI = std::unique_ptr<QT>(new QT {filepath});
    }
}

void QT::error(const std::string& errormessage) const { throw std::runtime_error(errormessage); }

auto QT::run(int argc, char* argv[]) -> int { return window_initialise(argc, argv); }
