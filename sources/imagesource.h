#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "iframesource.h"

class ImageSource : public IFrameSource
{
public:
    bool open(const QString &source) override;
    bool read(cv::Mat &frame) override;
    void close() override;
    bool isOpened() const override;

private:
    bool opened = false;
    QString imagePath;
};

#endif // IMAGESOURCE_H
