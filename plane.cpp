#include "plane.h"

#include <QDebug>

qreal const INVISIBILITY_CONSTRAINT = 1e-6;
qreal const MIN_NORMAL_LENGTH = 1e-10;

Plane::Plane(QVector3D normal, QVector3D planePoint)
    : isSingular_(false)
{
    if (normal.length() < MIN_NORMAL_LENGTH) {
        throw new ZeroNormalVectorException;
    }
    normal.normalize();
   // qDebug() << "normal " << normal;
    if (qAbs(normal.z()) < INVISIBILITY_CONSTRAINT) {
        isSingular_ = true;
        return;
    }
    normal /= normal.z();

    a = -normal.x();
    b = -normal.y();
    c = QVector3D::dotProduct(normal, planePoint);
}

Plane::Plane(QVector3D firstPoint, QVector3D secondPoint, QVector3D thirdPoint) :
    Plane(QVector3D::crossProduct(thirdPoint - firstPoint, secondPoint - firstPoint),
        firstPoint)
{
    //qDebug() << "Plane" << firstPoint << secondPoint << thirdPoint;
    //qDebug() << a << b << c;
}

Plane Plane::anyPlaneThroughPoints(QVector<QVector3D> points) {
    if (points.size() == 2) {
        return anyPlaneThroughTwoPoints(points[0], points[1]);
    }
    else {
        return Plane(points[0], points[1], points[2]);
    }
}

Plane Plane::anyPlaneThroughTwoPoints(QVector3D firstPoint, QVector3D secondPoint) {
    QVector3D direction = (secondPoint - firstPoint).normalized();
    QVector3D shift(1, 1, 1);
    if (QVector3D::crossProduct(direction, shift).length() < 0.01) {
        shift = QVector3D(1, -1, 1);
    }
    return Plane(firstPoint, secondPoint, firstPoint + shift);
}

qreal Plane::calculateZ(qreal x, qreal y) {
    Q_ASSERT(!isSingular());
    return a * x + b * y + c;
}

qreal Plane::calculateZ(QPoint point) {
    return calculateZ(point.x(), point.y());
}

bool Plane::isSingular() const {
    return isSingular_;
}
