#ifndef UI_H
#define UI_H

#include <filesystem>
#include <memory>
#include <string>

class UserInterface {
protected:
    std::string filepath;
    UserInterface(const std::string& filepath) : filepath {filepath} {}

public:
    virtual std::string get_rom_filepath() const = 0;
    virtual void error(const std::string&) const = 0;
};

inline std::unique_ptr<UserInterface> UI;


class CLI : public UserInterface {
    CLI(const std::string&);

public:
    static void set_ui(const std::string& filepath);
    std::string get_rom_filepath() const override;
    void error(const std::string&) const override;
};

class QT : public UserInterface {
    QT();

public:
    static void set_ui();
    std::string get_rom_filepath() const override;
    void error(const std::string&) const override;
};

#endif
