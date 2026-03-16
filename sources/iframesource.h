#ifndef IFRAMESOURCE_H
#define IFRAMESOURCE_H

#include <QImage>
#include <QString>

class IFrameSource
{
public:
    virtual ~IFrameSource() = default;

    virtual bool open(const QString &source) = 0;
    virtual bool read(QImage &frame) = 0;
    virtual void close() = 0;
    virtual bool isOpened() const = 0;
};

#endif // IFRAMESOURCE_H
