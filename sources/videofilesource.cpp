#include "videofilesource.h"

#include <opencv2/videoio.hpp>

VideoFileSource::VideoFileSource()
    : capture(new cv::VideoCapture())
{
}

VideoFileSource::~VideoFileSource()
{
    close();
    delete capture;
}

bool VideoFileSource::open(const QString &source)
{
    close();
    return capture->open(source.toStdString());
}

bool VideoFileSource::read(cv::Mat &frame)
{
    if (!capture->isOpened()) {
        return false;
    }

    return capture->read(frame);
}

void VideoFileSource::close()
{
    if (capture->isOpened()) {
        capture->release();
    }
}

bool VideoFileSource::isOpened() const
{
    return capture->isOpened();
}
