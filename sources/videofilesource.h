#ifndef VIDEOFILESOURCE_H
#define VIDEOFILESOURCE_H

#include "iframesource.h"

namespace cv {
class VideoCapture;
}

class VideoFileSource : public IFrameSource
{
public:
    VideoFileSource();
    ~VideoFileSource() override;

    bool open(const QString &source) override;
    bool read(cv::Mat &frame) override;
    void close() override;
    bool isOpened() const override;

private:
    cv::VideoCapture *capture;
};

#endif // VIDEOFILESOURCE_H
