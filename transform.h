#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QVector3D>

class QDebug;

class Transform
{
    friend QDebug operator<<(QDebug, Transform const&);
public:
    Transform();

    QVector3D apply(QVector3D const& arg) const;

    static Transform doNothing();
    static Transform shift(QVector3D const& onVector);
    static Transform reflect(quint8 axeNumber);
    static Transform orthogonal(QVector3D toI, QVector3D toJ, QVector3D toK);
    static Transform scale(qreal kx, qreal ky, qreal kz);
    static Transform perspective(qreal zMin);

    Transform combine(Transform const& other);

private:
    void setColumn(quint8 index, QVector3D vector);

private:
    double matrix [4][4];
};

#endif // TRANSFORM_H
