#ifndef DETECTIONRESULT_H
#define DETECTIONRESULT_H

#include <QRect>
#include <QString>
#include <QVector>

struct DetectionBox
{
    QRect rect;
    QString className;
    float confidence = 0.0f;
};

struct DetectionResult
{
    QVector<DetectionBox> boxes;
    int detectionsCount = 0;
    double inferenceMs = 0.0;
};

#endif // DETECTIONRESULT_H
