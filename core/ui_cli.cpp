#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include "ui.h"

CLI::CLI(const std::string& filepath) : UserInterface(filepath) {}

void CLI::set_ui(const std::string& filepath) {
    if (UI) UI->error("UI has already been set");
    UI = std::unique_ptr<CLI>(new CLI {filepath});
}

std::string CLI::get_rom_filepath() const {
    if (!std::filesystem::exists(filepath)) error("File doesn't exist");
    return filepath;
}

void CLI::error(const std::string& errormessage) const { throw std::runtime_error(errormessage); }
