#include <qquickstyle.h>
#include <QGuiApplication>
#include <QImage>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "qt/imageprovider.h"
#include "qmlbridge.h"

auto window_initialise(int argc, char* argv[]) -> int {
    QGuiApplication app {argc, argv};
    QQmlApplicationEngine engine {};

    // Objects automatically deleted by engine
    QmlBridge* qmlbridge {new QmlBridge};
    // TODO: Register in UI object
    engine.setInitialProperties({{"qmlbridge", QVariant::fromValue(qmlbridge)}});
    engine.addImageProvider("generated", new ImageProvider);

    engine.load(QUrl {"qrc:/mainwindow/mainwindow.qml"});
    return app.exec();
}
