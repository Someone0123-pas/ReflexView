#include "qmlbridge.h"

#include <QString>
#include <QUrl>
#include <stdexcept>

#include "ui.h"

auto QmlBridge::get_rom_filepath() const -> QString {
    try {
        QString filepath {QString::fromStdString(UI->get_rom_filepath())};
        emit success_get_rom_filepath();
        return filepath;
    } catch (std::runtime_error& e) { emit error("Error!", e.what()); }
    return "";
}

void QmlBridge::set_rom_filepath(const QUrl& filepath) const {
    try {
        UI->set_rom_filepath(filepath.toLocalFile().toStdString());
    } catch (std::runtime_error& e) { emit error("Error!", e.what()); }
    emit success_set_rom_filepath();
}

auto QmlBridge::is_filepath_loaded() const -> bool { return UI->is_filepath_loaded(); }
