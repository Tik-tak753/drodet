#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <QImage>

#include "detection/detectionresult.h"

namespace cv {
class Mat;
}

class FrameProcessor
{
public:
    static QImage matToQImage(const cv::Mat &frame);
    static cv::Mat drawDetections(const cv::Mat &frame, const DetectionResult &result);
    static cv::Mat drawOverlay(const cv::Mat &frame, double fps, int detections);
};

#endif // FRAMEPROCESSOR_H
