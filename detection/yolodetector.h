#ifndef YOLODETECTOR_H
#define YOLODETECTOR_H

#include <QString>

#include "detectionresult.h"

namespace cv {
class Mat;
}

class YoloDetector
{
public:
    bool loadModel(const QString &path);
    bool isLoaded() const;
    DetectionResult detect(const cv::Mat &frame, float conf, float iou, int imgsz);

private:
    bool modelLoaded = false;
    QString modelPath;
};

#endif // YOLODETECTOR_H
