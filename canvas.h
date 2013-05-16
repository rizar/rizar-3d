#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QScopedPointer>
#include <QPoint>
#include <QVector>

class QPaintDevice;
class QPainter;
class QImage;

class IPainter {
public:
    virtual ~IPainter() {}

    virtual void drawPixel(QPoint point, qreal z) = 0;
    virtual QColor color() = 0;
    virtual void setColor(QColor color) = 0;
};

class ICanvas
{
public:
    virtual ~ICanvas() {}

    virtual quint32 width() = 0;
    virtual quint32 height() = 0;

    virtual IPainter * createPainter() = 0;
    virtual void flush() = 0;
};

class MocCanvas : public ICanvas {
public:
    MocCanvas(quint32 width, quint32 height);

    virtual quint32 width();
    virtual quint32 height();

    virtual IPainter * createPainter();
    virtual void flush();

private:
    quint32 width_;
    quint32 height_;
};

class CanvasBase : public ICanvas {
public:
    CanvasBase(QPaintDevice * device);

    virtual quint32 width();
    virtual quint32 height();

    QPaintDevice * device();

    virtual void flush();
    void setPixel(QPoint const& pos, uint rgb);

private:
    QPaintDevice * device_;
    QScopedPointer<QImage> buffer_;
    uint * bufferBits_;
    qint32 width_;
};

class SimpleCanvas : public CanvasBase {
    friend class SimplePainter;
public:
    SimpleCanvas(QPaintDevice * device);

    virtual IPainter * createPainter();
};

class ZBufferCanvas : public CanvasBase {
    friend class ZBufferPainter;
public:
    ZBufferCanvas(QPaintDevice * device);

    virtual IPainter * createPainter();

private:
    qreal& depth(QPoint point);

private:
    QVector< QVector<qreal> > zBuffer_;
};

#endif // CANVAS_H
