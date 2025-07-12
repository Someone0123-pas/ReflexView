#include <QGuiApplication>
#include <QImage>
#include <QQmlApplicationEngine>

#include "qt/imageprovider.h"
#include "structviewer/background.h"

auto window_initialise(int argc, char* argv[]) -> int {
    QGuiApplication app {argc, argv};
    QQmlApplicationEngine engine {};

    // Deleted automatically by engine afterwards
    engine.addImageProvider("generated", new ImageProvider);

    engine.load(QUrl {"qrc:/mainwindow/mainwindow.qml"});
    return app.exec();
}
