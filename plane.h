#ifndef PLANE_H
#define PLANE_H

#include <QVector>
#include <QVector3D>
#include <QPoint>

class ZeroNormalVectorException {
};

class Plane {
public:
    Plane(QVector3D normal, QVector3D planePoint);
    Plane(QVector3D firstPoint, QVector3D secondPoint, QVector3D thirdPoint);

    static Plane anyPlaneThroughPoints(QVector<QVector3D> points);
    static Plane anyPlaneThroughTwoPoints(QVector3D firstPoint, QVector3D secondPoint);

    bool isSingular() const;

    qreal calculateZ(qreal x, qreal y);
    qreal calculateZ(QPoint point);

private:
    bool isSingular_;
    qreal a, b, c;
};

#endif // PLANE_H
