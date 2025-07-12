#include "qt/imageprovider.h"

#include <QSize>
#include <QString>
#include <stdexcept>

#include "structviewer/background.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

auto ImageProvider::image_from_id(const QString& id) const -> QImage {
    return QImage::fromData(
        reinterpret_cast<const unsigned char*>(images.at(id).first.get()), static_cast<int>(images.at(id).second),
        "PNG"
    );
}

auto ImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) -> QImage {
    if (images.contains(id)) {
        return image_from_id(id);
    }

    if (id.startsWith("background")) {
        images.try_emplace(id, BackgroundView {id.sliced(id.size() - 2).toUInt()}.get_png());
    }
    // Insert other PNG structs here
    else {
        // TODO: Better error handling with QML
        UI->error("The internally requested image is not associated with any type.");
        throw std::runtime_error("Unknown requested image");
    }

    return image_from_id(id);
}
