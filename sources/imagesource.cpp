#include "imagesource.h"

bool ImageSource::open(const QString &source)
{
    imagePath = source;
    cachedImage = QImage(imagePath);
    opened = !imagePath.isEmpty() && !cachedImage.isNull();
    return opened;
}

bool ImageSource::read(QImage &frame)
{
    if (!opened || cachedImage.isNull()) {
        return false;
    }

    frame = cachedImage;
    return true;
}

void ImageSource::close()
{
    opened = false;
    imagePath.clear();
    cachedImage = QImage();
}

bool ImageSource::isOpened() const
{
    return opened;
}
