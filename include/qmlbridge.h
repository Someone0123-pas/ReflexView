#ifndef APPBRIDGE_QT_H
#define APPBRIDGE_QT_H

#include <QtQml/qqmlregistration.h>

#include <QObject>
#include <QString>
#include <QUrl>

class QmlBridge : public QObject {
    Q_OBJECT
    QML_ELEMENT

public slots:
    auto get_rom_filepath() const -> QString;
    void set_rom_filepath(const QUrl& filepath) const;
    auto is_filepath_loaded() const -> bool;

signals:
    void error(const QString& errortitle, const QString& errormessage) const;
    void success_set_rom_filepath() const;
    void success_get_rom_filepath() const;
};

#endif  // APPBRIDGE_QT_H
