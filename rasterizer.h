#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QVector>
#include <QPoint>

class ICanvas;

class Rasterizer
{
public:
    Rasterizer(ICanvas * canvas);

    bool clearDestination() const;
    Rasterizer & setClearDestination(bool newValue);

    void rasterizeLine(QPoint begin, QPoint end, QVector<QPoint> * pixels) const;

private:
    void rasterizeVerticalLine(QPoint begin, QPoint end, QVector<QPoint> * pixels) const;

private:
    bool clearDestination_;
    ICanvas * canvas_;
};

#endif // RASTERIZER_H
