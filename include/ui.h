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
    virtual auto get_rom_filepath() const -> std::string = 0;
    virtual void error(const std::string&) const = 0;
    virtual auto run(int argc, char* argv[]) -> int = 0;
};

// If core-functions need user-provided information, it should always run through this interface
inline std::unique_ptr<UserInterface> UI;

class CLI : public UserInterface {
    CLI(const std::string&);

public:
    ~CLI() override = default;
    static void set_ui(const std::string& filepath);
    auto get_rom_filepath() const -> std::string override;
    void error(const std::string&) const override;
    auto run(int argc, char* argv[]) -> int override;
};

class QT : public UserInterface {
    QT() = default;
    QT(const std::string&);

public:
    ~QT() override = default;
    static void set_ui();
    static void set_ui(const std::string& filepath);
    auto get_rom_filepath() const -> std::string override;
    void set_rom_filepath(const std::string& filepath);
    void error(const std::string&) const override;
    auto run(int argc, char* argv[]) -> int override;
};

#endif
