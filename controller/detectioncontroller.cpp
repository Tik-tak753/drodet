#include "detectioncontroller.h"

#include "sources/imagesource.h"

DetectionController::DetectionController(QObject *parent)
    : QObject(parent)
{
    emit statusChanged(QStringLiteral("Controller initialized"));
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

    if (frameSource->open(path)) {
        emit statusChanged(QStringLiteral("Image source opened"));
    } else {
        emit statusChanged(QStringLiteral("Failed to open image source"));
    }
}

void DetectionController::stop()
{
    if (frameSource) {
        frameSource->close();
        frameSource.reset();
    }

    emit statusChanged(QStringLiteral("Stopped"));
}
