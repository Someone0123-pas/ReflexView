#ifndef DEBUG_QT_H
#define DEBUG_QT_H

#include <QString>
#include <QQmlApplicationEngine>

namespace qtdebug {
void print_qrc_directories();
void check_qrc_file(const QString& filepath);
void print_qml_import_paths(const QQmlApplicationEngine&);
}  // namespace qtdebug

#endif  // DEBUG_QT_H
