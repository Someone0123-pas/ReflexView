#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

#include "byteviewer.h"
#include "structviewer/background.h"
#include "types.h"
#include "ui.h"

auto UserInterface::get_rom_filepath() const -> std::string {
    if (!std::filesystem::exists(filepath)) {
        error("Error: Path", std::format("The file with path {} doesn't exist.", filepath));
    }
    return filepath;
}

void UserInterface::set_rom_filepath(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        error("Error: Path", std::format("The file with path {} doesn't exist.", filepath));
        return;
    }

    const std::string old_filepath {this->filepath};
    try {
        this->filepath = filepath;
        Byteviewer::load_rom();
    } catch (std::runtime_error& e) {
        this->filepath = old_filepath;
        throw;
    }
}

auto UserInterface::is_filepath_loaded() const -> bool { return !filepath.empty(); }

CLI::CLI(const std::string& filepath) : UserInterface(filepath) {}

void CLI::set_ui(const std::string& filepath) {
    if (UI) {
        UI->error("Internal Error: UI", "The UI has already been set.");
    }
    UI = std::unique_ptr<CLI>(new CLI {filepath});

    if (!std::filesystem::exists(filepath)) {
        UI->error("Error: Path", std::format("The file with path {} doesn't exist.", filepath));
    }
}

namespace arg {
enum CommandType : u8 { BACKGROUNDS, BACKGROUND_INDEX, BPP4, BPP4_INDEX };
struct Command {
    CommandType command {};
    int index {-1};
};

static std::filesystem::path outputdir {};

static void help();
static void backgrounds();
static void backgrounds(unsigned index);
static void bpp4();
static void bpp4(unsigned index);
}  // namespace arg

void CLI::error(const std::string& errortitle, const std::string& errormessage) const {
    std::println(std::cerr, "{}\n{}", errortitle, errormessage);
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
                UI->error("Error: Arguments", "No outputdirectory was specified.");
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

        else if (*argitr == "-4") {
            if (argitr + 1 == args.end()) {
                commands.emplace_back(arg::BPP4);
            } else
                try {
                    int index {std::stoi(*(argitr + 1))};
                    commands.emplace_back(arg::BPP4_INDEX, index);
                    ++argitr;
                } catch (...) { commands.emplace_back(arg::BPP4); }
        }

        else {
            UI->error("Error: Arguments", "One of the arguments was invalid.");
        }
    }

    if (commands.empty()) {
        UI->error("Error: Arguments", "Nothing to do!");
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
            case arg::BPP4:
                arg::bpp4();
                break;
            case arg::BPP4_INDEX:
                arg::bpp4(cmd.index);
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
        "   -4 [INDEX]\t\t\t dump background as .4bpp file with INDEX (0-35 or all, if not specified)"
    );
    std::println(
        "   -b [INDEX]\t\t\t dump background as coloured, indexed PNG with INDEX (0-35 or all, if not "
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

static void arg::bpp4() {
    for (unsigned index {}; index <= BackgroundView::max_index; ++index) { bpp4(index); }
}

static void arg::bpp4(unsigned index) {
    BackgroundView {index}.dump_tileset_4bpp(arg::outputdir / std::format("background-{:02}", index));
}
