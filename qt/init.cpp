#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>

#include "qt/debug.h"

auto initialise(int argc, char* argv[]) -> int {
    QGuiApplication app {argc, argv};
    QQmlApplicationEngine engine {};
    const QUrl mainwindow {"qrc:/mainwindow/init.qml"};

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );
    engine.load(mainwindow);

    return app.exec();
}
