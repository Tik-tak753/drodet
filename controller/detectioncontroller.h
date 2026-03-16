#ifndef DETECTIONCONTROLLER_H
#define DETECTIONCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>

#include "detection/yolodetector.h"

class IFrameSource;
class QImage;

class DetectionController : public QObject
{
    Q_OBJECT

public:
    explicit DetectionController(QObject *parent = nullptr);

public slots:
    void loadModel(const QString &path);
    void openImage(const QString &path);
    void stop();

signals:
    void frameReady(const QImage &frame);
    void statsReady(double fps, int detections);
    void statusChanged(const QString &status);

private:
    YoloDetector detector;
    QScopedPointer<IFrameSource> frameSource;
};

#endif // DETECTIONCONTROLLER_H
