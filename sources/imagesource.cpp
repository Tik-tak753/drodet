#include "imagesource.h"

#include <opencv2/imgcodecs.hpp>

bool ImageSource::open(const QString &source)
{
    imagePath = source;
    opened = !imagePath.isEmpty();
    return opened;
}

bool ImageSource::read(cv::Mat &frame)
{
    if (!opened) {
        return false;
    }

    frame = cv::imread(imagePath.toStdString());
    opened = false;
    return !frame.empty();
}

void ImageSource::close()
{
    opened = false;
    imagePath.clear();
}

bool ImageSource::isOpened() const
{
    return opened;
}
