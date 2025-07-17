#ifndef UI_QT_H
#define UI_QT_H

#include <QtQml/qqmlregistration.h>

#include <QObject>

#include "ui.h"

class QT : public UserInterface {
    QT() = default;
    QT(const std::string&);

public:
    ~QT() override = default;
    static void set_ui();
    static void set_ui(const std::string& filepath);
    void error(const std::string& errortitle, const std::string& errormessage) const override;
    auto run(int argc, char* argv[]) -> int override;
};

#endif  // UI_QT_H
