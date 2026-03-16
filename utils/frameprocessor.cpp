#include "frameprocessor.h"

QImage FrameProcessor::matToQImage(const cv::Mat &frame)
{
    Q_UNUSED(frame)

    // TODO: implement cv::Mat -> QImage conversion
    return QImage();
}

cv::Mat FrameProcessor::drawDetections(const cv::Mat &frame, const DetectionResult &result)
{
    Q_UNUSED(result)

    // TODO: implement detection box drawing
    return frame;
}

cv::Mat FrameProcessor::drawOverlay(const cv::Mat &frame, double fps, int detections)
{
    Q_UNUSED(fps)
    Q_UNUSED(detections)

    // TODO: implement FPS and detection overlay drawing
    return frame;
}
