#include "imagesource.h"

bool ImageSource::open(const QString &source)
{
    imagePath = source;
    opened = !imagePath.isEmpty();
    return opened;
}

bool ImageSource::read(cv::Mat &frame)
{
    Q_UNUSED(frame)

    // TODO: implement real image frame loading here
    return false;
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
