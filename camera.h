#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include "transform.h"

class IBody;
class ICanvas;

class ICamera
{
public:
    ICamera(ICanvas * canvas);
    ~ICamera() {}

    ICanvas * canvas() const;
    void setCanvas(ICanvas * canvas);

    Transform const& toCanonical() const;
    Transform const& toUser() const;

    virtual void draw(IBody const& body) const = 0;

    virtual void reset() = 0;
    virtual void moveForward(qreal step) = 0;
    virtual void moveBackward(qreal step);
    virtual void moveLeft(qreal step) = 0;
    virtual void moveRight(qreal step);
    virtual void turnLeft(qreal angle) = 0;
    virtual void turnRight(qreal angle);
    virtual void turnUp(qreal angle) = 0;
    virtual void turnDown(qreal angle);

    virtual qreal latitude() = 0;
    virtual qreal longitude() = 0;
    virtual qreal height() = 0;
    virtual qreal zenith() = 0;
    virtual qreal azimuth() = 0;

protected:
    void setToCanonical(Transform const& transform);
    void setToUser(Transform const& toUser);
    virtual void buildTransform() = 0;

private:
    ICanvas * canvas_;
    Transform toCanonical_;
    Transform toUser_;
};

class CentralProjectionCamera : public ICamera {
public:
    CentralProjectionCamera(QVector3D cop = QVector3D(0, 0, 0),
                            QVector3D vpn = QVector3D(0, 0, 1),
                            qreal near = 10,
                            qreal far = 100);

    virtual void reset();

    virtual void moveForward(qreal step);
    virtual void moveLeft(qreal step);

    virtual void turnLeft(qreal angle);
    virtual void turnUp(qreal angle);

    virtual qreal latitude();
    virtual qreal longitude();
    virtual qreal height();
    virtual QVector3D position();
    virtual qreal zenith();
    virtual qreal azimuth();

    virtual void draw(const IBody &body) const;

private:
    virtual void buildTransform();

private:
    QVector3D cop_; // center of projection
    QVector3D vpn_; // view plane normal
    QVector3D vup_; // vertical vector
    qreal near_;
    qreal far_;

    QVector3D initialCop_;
    QVector3D initialVpn_;
};

#endif // CAMERA_H
