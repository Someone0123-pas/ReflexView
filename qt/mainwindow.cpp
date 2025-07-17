#include <qquickstyle.h>

#include <QGuiApplication>
#include <QImage>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "qmlbridge.h"
#include "qt/imageprovider.h"

auto window_initialise(int argc, char* argv[]) -> int {
    QGuiApplication app {argc, argv};
    QQmlApplicationEngine engine {};

    // Objects automatically deleted by engine
    QmlBridge* qmlbridge {new QmlBridge};
    ImageProvider* imageprovider {new ImageProvider};
    imageprovider->set_errorhandler(qmlbridge);

    engine.setInitialProperties({{"qmlbridge", QVariant::fromValue(qmlbridge)}});
    engine.addImageProvider("generated", imageprovider);

    engine.load(QUrl {"qrc:/mainwindow/mainwindow.qml"});
    return app.exec();
}
