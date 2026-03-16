#include "detectioncontroller.h"

#include "sources/imagesource.h"

#include <QImage>

DetectionController::DetectionController(QObject *parent)
    : QObject(parent)
{
}

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
    frameSource.reset(new ImageSource());

    if (!frameSource->open(path)) {
        emit statusChanged(QStringLiteral("Failed to open image source"));
        return;
    }

    QImage frame;
    if (!frameSource->read(frame)) {
        emit statusChanged(QStringLiteral("Failed to read image frame"));
        return;
    }

    emit frameReady(frame);
    emit statsReady(0.0, 0);
    emit statusChanged(QStringLiteral("Image loaded"));
}

void DetectionController::stop()
{
    if (frameSource) {
        frameSource->close();
        frameSource.reset();
    }

    emit statusChanged(QStringLiteral("Stopped"));
}
