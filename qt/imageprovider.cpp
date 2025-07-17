#include "qt/imageprovider.h"

#include <QSize>
#include <QString>
#include <iostream>
#include <print>
#include <stdexcept>

#include "structviewer/background.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

auto ImageProvider::image_from_id(const QString& id) const -> QImage {
    return QImage::fromData(
        reinterpret_cast<const unsigned char*>(images.at(id).first.get()),
        static_cast<int>(images.at(id).second), "PNG"
    );
}

auto ImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) -> QImage {
    if (!errorhandler) {
        std::println(
            std::cerr,
            "Internal Error: Wrong logic\nThe program is ill-formed, ImageProvider was instantiated without "
            "setting an errorhandler QmlBridge."
        );
        std::exit(1);
    }

    if (images.contains(id)) {
        return image_from_id(id);
    }

    if (const auto& delimiterindex {id.lastIndexOf('-')};
        delimiterindex != -1 && id.startsWith("background")) {
        try {
            const BackgroundView background {id.sliced(delimiterindex + 1).toUInt()};
            images.try_emplace(id, background.get_png());
        } catch (std::runtime_error& e) {
            emit errorhandler->error(e.what());
            return {};
        }
    }

    // Insert other PNG structs here

    else {
        emit errorhandler->error("The internally requested image is not associated with any type.");
    }

    return image_from_id(id);
}

void ImageProvider::set_errorhandler(QmlBridge* errorhandler) { this->errorhandler = errorhandler; }
