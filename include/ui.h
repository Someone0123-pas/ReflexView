#ifndef UI_H
#define UI_H

#include <memory>
#include <string>
#include <utility>

class UserInterface {
protected:
    std::string filepath;
    UserInterface() = default;
    UserInterface(std::string filepath) : filepath {std::move(filepath)} {}

public:
    virtual ~UserInterface() = default;
    virtual auto get_rom_filepath() const -> std::string;
    virtual void set_rom_filepath(const std::string&);
    virtual auto is_filepath_loaded() const -> bool;
    virtual void error(const std::string& errortitle, const std::string& errormessage) const = 0;
    virtual auto run(int argc, char* argv[]) -> int = 0;
};

// If core-functions need user-provided information, it should always run through this interface
inline std::unique_ptr<UserInterface> UI;

class CLI : public UserInterface {
    CLI(const std::string&);

public:
    ~CLI() override = default;
    static void set_ui(const std::string& filepath);
    void error(const std::string& errortitle, const std::string& errormessage) const override;
    auto run(int argc, char* argv[]) -> int override;
};

#endif
