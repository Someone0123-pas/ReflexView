#include "appbridge.h"
#include "ui.h"

#include <QString>
#include <QUrl>

auto AppBridge::get_rom_filepath() const -> QString { return QString::fromStdString(UI->get_rom_filepath()); }
void AppBridge::set_rom_filepath(const QUrl& filepath) const { UI->set_rom_filepath(filepath.toLocalFile().toStdString()); }
auto AppBridge::is_filepath_loaded() const -> bool { return UI->is_filepath_loaded(); }
