#include "yolodetector.h"

bool YoloDetector::loadModel(const QString &path)
{
    modelPath = path;
    modelLoaded = !modelPath.isEmpty();
    return modelLoaded;
}

bool YoloDetector::isLoaded() const
{
    return modelLoaded;
}

DetectionResult YoloDetector::detect(const cv::Mat &frame, float conf, float iou, int imgsz)
{
    Q_UNUSED(frame)
    Q_UNUSED(conf)
    Q_UNUSED(iou)
    Q_UNUSED(imgsz)

    // TODO: implement real YOLO inference here
    return DetectionResult{};
}
