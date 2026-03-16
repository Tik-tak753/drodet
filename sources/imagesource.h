#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include <QImage>

#include "iframesource.h"

class ImageSource : public IFrameSource
{
public:
    bool open(const QString &source) override;
    bool read(QImage &frame) override;
    void close() override;
    bool isOpened() const override;

private:
    bool opened = false;
    QString imagePath;
    QImage cachedImage;
};

#endif // IMAGESOURCE_H
