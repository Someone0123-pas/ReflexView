#ifndef APPBRIDGE_QT_H
#define APPBRIDGE_QT_H

#include <QtQml/qqmlregistration.h>

#include <QObject>
#include <QString>
#include <QUrl>

class AppBridge : public QObject {
    Q_OBJECT
    QML_ELEMENT
public slots:
    auto get_rom_filepath() const -> QString;
    void set_rom_filepath(const QUrl& filepath) const;
    auto is_filepath_loaded() const -> bool;
};

#endif  // APPBRIDGE_QT_H
