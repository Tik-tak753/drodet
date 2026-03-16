#include "detectioncontroller.h"

#include "sources/iframesource.h"
#include "sources/imagesource.h"
#include "sources/videofilesource.h"
#include "utils/frameprocessor.h"

#include <QDateTime>

#include <opencv2/core/mat.hpp>

DetectionController::DetectionController(QObject *parent)
    : QObject(parent)
{
    connect(&previewTimer, &QTimer::timeout, this, &DetectionController::processNextFrame);
    previewTimer.setInterval(33);

    emit statusChanged(QStringLiteral("Controller initialized"));
}

DetectionController::~DetectionController() = default;

void DetectionController::loadModel(const QString &path)
{
    if (detector.loadModel(path)) {
        emit statusChanged(QStringLiteral("Model loaded"));
    } else {
        emit statusChanged(QStringLiteral("Failed to load model"));
    }
}

void DetectionController::openImage(const QString &path)
{
    previewTimer.stop();
    if (frameSource) {
        frameSource->close();
    }

    frameSource.reset(new ImageSource());

    if (frameSource->open(path)) {
        emit statusChanged(QStringLiteral("Image source opened"));
    } else {
        emit statusChanged(QStringLiteral("Failed to open image source"));
    }
}

void DetectionController::openVideo(const QString &path)
{
    previewTimer.stop();
    if (frameSource) {
        frameSource->close();
    }

    frameSource.reset(new VideoFileSource());

    if (frameSource->open(path)) {
        emit statusChanged(QStringLiteral("Video source opened"));
    } else {
        emit statusChanged(QStringLiteral("Failed to open video source"));
    }
}

void DetectionController::start()
{
    if (!frameSource || !frameSource->isOpened()) {
        emit statusChanged(QStringLiteral("No opened source to start"));
        return;
    }

    processedFrames = 0;
    startedAtMs = QDateTime::currentMSecsSinceEpoch();
    previewTimer.start();
    emit statusChanged(QStringLiteral("Preview started"));
}

void DetectionController::stop()
{
    previewTimer.stop();

    processedFrames = 0;
    startedAtMs = 0;
    emit statsReady(0.0, 0);
    emit statusChanged(QStringLiteral("Stopped"));
}

void DetectionController::processNextFrame()
{
    if (!frameSource || !frameSource->isOpened()) {
        stop();
        return;
    }

    cv::Mat frame;
    if (!frameSource->read(frame)) {
        previewTimer.stop();
        if (frameSource) {
            frameSource->close();
        }

        emit statsReady(0.0, 0);
        emit statusChanged(QStringLiteral("Preview ended"));
        return;
    }

    const QImage image = FrameProcessor::matToQImage(frame);
    if (!image.isNull()) {
        emit frameReady(image);
    }

    ++processedFrames;
    const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch() - startedAtMs;
    const double fps = (elapsedMs > 0) ? (1000.0 * static_cast<double>(processedFrames) / static_cast<double>(elapsedMs)) : 0.0;

    // TODO: implement real YOLO inference here
    emit statsReady(fps, 0);
}
