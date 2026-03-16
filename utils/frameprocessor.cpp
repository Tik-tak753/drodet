#include "frameprocessor.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

QImage FrameProcessor::matToQImage(const cv::Mat &frame)
{
    if (frame.empty()) {
        return QImage();
    }

    if (frame.channels() == 3) {
        cv::Mat rgb;
        cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }

    if (frame.channels() == 1) {
        return QImage(frame.data,
                      frame.cols,
                      frame.rows,
                      static_cast<int>(frame.step),
                      QImage::Format_Grayscale8)
            .copy();
    }

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
