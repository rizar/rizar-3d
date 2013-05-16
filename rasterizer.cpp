#include "rasterizer.h"
#include "canvas.h"

#include <QDebug>

Rasterizer::Rasterizer(ICanvas * canvas) :
    clearDestination_(true),
    canvas_(canvas)
{
    Q_ASSERT(canvas);
}

bool Rasterizer::clearDestination() const {
    return clearDestination_;
}

Rasterizer & Rasterizer::setClearDestination(bool newValue) {
    clearDestination_ = newValue;
    return *this;
}

void Rasterizer::rasterizeLine(QPoint begin, QPoint end, QVector<QPoint> * pixels) const {
    Q_ASSERT(pixels);
    if (clearDestination()) {
        pixels->clear();
    }

    if (begin.x() == end.x()) {
        rasterizeVerticalLine(begin, end, pixels);
        return;
    }
    if (begin.x() > end.x()) {
        qSwap(begin, end);
    }

    qreal k = (end.y() - begin.y()) / (qreal)(end.x() - begin.x());
    qreal b = begin.y() - begin.x() * k;

    if (begin.x() < 0) {
        begin.setX(0);
        begin.setY(b);
    }
    if (end.x() >= (qint32)canvas_->width()) {
        end.setX(canvas_->width() - 1);
        end.setY(k * end.x() + b);
    }

    qint32 x = begin.x();
    qint32 y = begin.y();

    qint32 delta = k < 0 ? -1 : 1;
    qreal err = 0;
    while (x <= end.x()) {
       if (err >= 0.5) {
           y += delta;
           err -= 1;
        }

        if (0 <= y && y < qint32(canvas_->height())) {
            pixels->push_back(QPoint(x, y));
        }

        if (err < 0.5) {
            x++;
            err += qAbs(k);
        }
    }
}

void Rasterizer::rasterizeVerticalLine(QPoint begin, QPoint end, QVector<QPoint> * pixels) const {
    if (begin.x() < 0 || begin.x() >= qint32(canvas_->width())) {
        return;
    }
    if (begin.y() > end.y()) {
        qSwap(begin, end);
    }

    for (qint32 y = qMax(0, begin.y());
         y <= qMin(end.y(), static_cast<qint32>(canvas_->height() - 1));
         y++)
    {
        pixels->push_back(QPoint(begin.x(), y));
    }
}
