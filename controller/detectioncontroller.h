#ifndef DETECTIONCONTROLLER_H
#define DETECTIONCONTROLLER_H

#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QTimer>

#include "detection/yolodetector.h"

class IFrameSource;
class QImage;

namespace cv {
class Mat;
}

class DetectionController : public QObject
{
    Q_OBJECT

public:
    explicit DetectionController(QObject *parent = nullptr);
    ~DetectionController() override;

public slots:
    void loadModel(const QString &path);
    void openImage(const QString &path);
    void openVideo(const QString &path);
    void start();
    void stop();

signals:
    void frameReady(const QImage &frame);
    void statsReady(double fps, int detections);
    void statusChanged(const QString &status);

private slots:
    void processNextFrame();

private:
    YoloDetector detector;
    QScopedPointer<IFrameSource> frameSource;
    QTimer previewTimer;
    int processedFrames = 0;
    qint64 startedAtMs = 0;
};

#endif // DETECTIONCONTROLLER_H
