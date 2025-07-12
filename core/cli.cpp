#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <vector>

#include "structviewer/background.h"
#include "types.h"
#include "ui.h"

auto UserInterface::get_rom_filepath() const -> std::string {
    if (!std::filesystem::exists(filepath)) {
        error("File doesn't exist");
    }
    return filepath;
}

void UserInterface::set_rom_filepath(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        error("File doesn't exist");
    }
    this->filepath = filepath;
}

auto UserInterface::is_filepath_loaded() const -> bool { return !filepath.empty(); }

CLI::CLI(const std::string& filepath) : UserInterface(filepath) {}

void CLI::set_ui(const std::string& filepath) {
    if (UI) {
        UI->error("UI has already been set");
    }
    UI = std::unique_ptr<CLI>(new CLI {filepath});

    if (!std::filesystem::exists(filepath)) {
        UI->error("File doesn't exist");
    }
}

namespace arg {
enum CommandType : u8 { BACKGROUNDS, BACKGROUND_INDEX };
struct Command {
    CommandType command {};
    int index {-1};
};

static std::filesystem::path outputdir {};

static void help();
static void backgrounds();
static void backgrounds(unsigned index);
}  // namespace arg

void CLI::error(const std::string& errormessage) const {
    std::println(std::cerr, "Error: {}", errormessage);
    arg::help();
    std::exit(1);
}

auto CLI::run(int argc, char* argv[]) -> int {
    std::vector<std::string> args {};
    std::vector<arg::Command> commands {};
    args.assign(argv + 2, argv + argc);

    // Two passes, because different commands have different priority

    for (auto argitr {args.begin()}; argitr != args.end(); ++argitr) {
        if (*argitr == "-h") {
            arg::help();
            return 0;
        }

        else if (*argitr == "-o") {
            if (argitr + 1 == args.end()) {
                UI->error("No outputdirectory specified");
            }
            arg::outputdir = *++argitr;
        }

        else if (*argitr == "-b") {
            if (argitr + 1 == args.end()) {
                commands.emplace_back(arg::BACKGROUNDS);
            } else
                try {
                    int index {std::stoi(*(argitr + 1))};
                    commands.emplace_back(arg::BACKGROUND_INDEX, index);
                    ++argitr;
                } catch (...) { commands.emplace_back(arg::BACKGROUNDS); }
        }

        // Invalid argument
        else {
            UI->error("Invalid argument");
        }
    }

    if (commands.empty()) {
        UI->error("Nothing to do!");
    }
    if (!arg::outputdir.empty()) {
        std::filesystem::create_directories(arg::outputdir);
    }

    for (const arg::Command& cmd : commands) {
        switch (cmd.command) {
            case arg::BACKGROUNDS:
                arg::backgrounds();
                break;
            case arg::BACKGROUND_INDEX:
                arg::backgrounds(cmd.index);
                break;
            default:
                assert(false);
        }
    }

    return 0;
}

static void arg::help() {
    std::println("\nReflexView: Print and dump information from Kirby and the Amazing Mirror");
    std::println("Usage:");
    std::println("  ./reflexview\t\t\tStarts QtQuick-Window");
    std::println("  ./reflexview <ROM>\t\tStarts QtQuick-Window with ROM already loaded\n");
    std::println("  ./reflexview <ROM> <OPTIONS>\tStarts CLI Mode with the following options:");
    std::println("   -h\t\t\t\t show this help and immediately exit");
    std::println(
        "   -b [INDEX]\t\t\t dump background as coloured, indexed PNG with INDEX (0-30 or all, if not "
        "specified),\n\t\t\t\t\t palette as .agbpal and tilemap as .tilemap"
    );
    std::println("   -o <DIR>\t\t\t dump all outputs into the specified directory");
}

static void arg::backgrounds() {
    for (unsigned index {}; index <= BackgroundView::max_index; ++index) { backgrounds(index); }
}

static void arg::backgrounds(unsigned index) {
    BackgroundView {index}.dump(arg::outputdir / std::format("background-{:02}", index));
}
