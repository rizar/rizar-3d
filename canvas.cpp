#include "canvas.h"

#include <QPaintDevice>
#include <QPainter>
#include <QDebug>
#include <QImage>

// MocPainter

class MocPainter : public IPainter {
public:
    virtual QColor color() {
        return QColor();
    }

    void setColor(QColor) {
    }

    virtual void drawPixel(QPoint, qreal) {
    }
};

// MocCanvas

MocCanvas::MocCanvas(quint32 width, quint32 height) :
    width_(width),
    height_(height)
{
}

quint32 MocCanvas::width() {
    return width_;
}

quint32 MocCanvas::height() {
    return height_;
}

IPainter * MocCanvas::createPainter() {
    return new MocPainter;
}

void MocCanvas::flush() {
}

// CanvasBase

CanvasBase::CanvasBase(QPaintDevice *device)
    : device_(device)
    , buffer_(new QImage(device_->width(), device_->height(), QImage::Format_RGB32))
{
    buffer_->fill(QColor("white"));
    bufferBits_ = reinterpret_cast<uint*>(buffer_->bits());
    width_ = width();
}

QPaintDevice * CanvasBase::device() {
    return device_;
}

quint32 CanvasBase::width() {
    return device_->width();
}

quint32 CanvasBase::height() {
    return device_->height();
}

void CanvasBase::flush() {
    QPainter painter(device());
    painter.drawImage(0, 0, *buffer_);
}

void CanvasBase::setPixel(const QPoint &pos, uint rgb) {
    quint32 index = pos.y() * width_ + pos.x();
    bufferBits_[index] = rgb;
}

// PainterBase

class PainterBase : public IPainter {
public:
    PainterBase(CanvasBase * canvas)
        : canvas_(canvas)
    {
    }

    virtual QColor color() {
        return QColor(colorRgb_);
    }

    virtual void setColor(QColor color) {
        colorRgb_ = color.rgb();
    }

    ICanvas * canvas() {
        return canvas_;
    }

protected:
    void doDrawPixel(QPoint point, qreal) {
        canvas_->setPixel(point, colorRgb_);
    }

private:
    uint colorRgb_;
    CanvasBase * canvas_;
};

// SimplePainter

class SimplePainter : public PainterBase {
public:
    SimplePainter(SimpleCanvas * canvas) :
        PainterBase(canvas)
    {
    }

    virtual void drawPixel(QPoint point, qreal z) {
        doDrawPixel(point, z);
    }
};

// SimpleCanvas

SimpleCanvas::SimpleCanvas(QPaintDevice * device) :
    CanvasBase(device)
{
}

IPainter * SimpleCanvas::createPainter() {
    return new SimplePainter(this);
}

// ZBufferPainter

class ZBufferPainter : public PainterBase {
public:
    ZBufferPainter(ZBufferCanvas * canvas)
        : PainterBase(canvas)
        , zBufferCanvas_(canvas)
    {
    }

    virtual void drawPixel(QPoint point, qreal z) {
        qreal & depth = zBufferCanvas_->depth(point);
        //qDebug() << point << "z" << z;
        if (depth > z && z > -1e-9) {
            depth = z;
            doDrawPixel(point, z);
        }
    }

private:
    // to avoid dynamic_casts
    ZBufferCanvas * zBufferCanvas_;
};

// ZBufferCanvas

qreal const MAX_Z_VALUE = 1e18;

ZBufferCanvas::ZBufferCanvas(QPaintDevice * device) :
    CanvasBase(device),
    zBuffer_(device->width(), QVector<qreal>(device->height(), MAX_Z_VALUE))
{
}

qreal& ZBufferCanvas::depth(QPoint point) {
    return zBuffer_[point.x()][point.y()];
}

IPainter * ZBufferCanvas::createPainter() {
    return new ZBufferPainter(this);
}
