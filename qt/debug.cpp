#include "qt/debug.h"

#include <QDirIterator>
#include <QStandardPaths>
#include <QString>

void qtdebug::print_qrc_directories() {
    QDirIterator it(":/", QDirIterator::Subdirectories);
    while (it.hasNext()) { qDebug() << "Resource:" << it.next(); }
}

void qtdebug::check_qrc_file(const QString& filepath) {
    if (QFile file {filepath}; file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        qDebug() << file.readAll();
        file.close();
    } else {
        qDebug() << "File \"" << filepath << "\" doesn't exist in the qrc-filesystem.\n";
    }
}

void qtdebug::print_qml_import_paths(const QQmlApplicationEngine& engine) {
    qDebug() << engine.importPathList();
}
