#ifndef UI_H
#define UI_H

#include <memory>
#include <string>
#include <utility>

class UserInterface {
protected:
    std::string filepath;
    UserInterface(std::string filepath) : filepath {std::move(filepath)} {}

public:
    virtual ~UserInterface() = default;
    virtual auto get_rom_filepath() const -> std::string = 0;
    virtual void error(const std::string&) const = 0;
};

inline std::unique_ptr<UserInterface> UI;


class CLI : public UserInterface {
    CLI(const std::string&);

public:
    ~CLI() override = default;
    static void set_ui(const std::string& filepath);
    auto get_rom_filepath() const -> std::string override;
    void error(const std::string&) const override;
};

class QT : public UserInterface {
    QT();

public:
    static void set_ui();
    auto get_rom_filepath() const -> std::string override;
    void error(const std::string&) const override;
};

#endif
