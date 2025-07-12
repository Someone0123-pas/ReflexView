#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QImage>
#include <QQuickImageProvider>
#include <QString>
#include <unordered_map>

class ImageProvider : public QQuickImageProvider {
    std::unordered_map<QString, std::pair<std::unique_ptr<const char[]>, const long>> images;
    auto image_from_id(const QString& id) const -> QImage;

public:
    ImageProvider();
    ~ImageProvider() override = default;
    auto requestImage(const QString& id, QSize* size, const QSize& requestedSize) -> QImage override;
};

#endif  // IMAGEPROVIDER_H