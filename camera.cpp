#include "camera.h"
#include "canvas.h"
#include "bodies.h"

#include <QDebug>

#include <cmath>

ICamera::ICamera(ICanvas * canvas = 0)
    : canvas_(canvas)
{
}

ICanvas * ICamera::canvas() const {
    return canvas_;
}

void ICamera::setCanvas(ICanvas * canvas) {
    canvas_ = canvas;
    buildTransform();
}

Transform const& ICamera::transform() const {
    return transform_;
}

void ICamera::setTransform(Transform const& transform)
{
    transform_ = transform;
}

void ICamera::draw(IBody const& body) {
    QScopedPointer<IBody> transformedBody(body.transform(transform()));
    transformedBody->draw(canvas());
}

void ICamera::moveBackward(qreal step) {
    moveForward(-step);
}

void ICamera::moveRight(qreal step) {
    moveLeft(-step);
}

void ICamera::turnDown(qreal angle) {
    turnUp(-angle);
}

void ICamera::turnRight(qreal angle) {
    turnLeft(-angle);
}

CentralProjectionCamera::CentralProjectionCamera(QVector3D cop,
                                                 QVector3D vpn,
                                                 qreal near,
                                                 qreal far) :
    cop_(cop),
    vpn_(vpn),
    vup_(0, 1, 0),
    near_(near),
    far_(far)
{
}

void CentralProjectionCamera::moveForward(qreal step)
{
    cop_ += step * vpn_;
}

void CentralProjectionCamera::moveLeft(qreal step)
{
    cop_ -= step * QVector3D::crossProduct(vpn_, vup_);
}

void CentralProjectionCamera::turnUp(qreal angle) {
    qreal curAngle = acos(0)  - acos(QVector3D::dotProduct(vpn_, vup_));
    QVector3D comp = (vpn_ - sin(curAngle) * vup_) / cos(curAngle);
    qreal newAngle = curAngle + angle;
    if (qAbs(newAngle) >= 2 * acos(0) / 3) {
        return;
    }
    vpn_ = cos(newAngle) * comp + sin(newAngle) * vup_;
}

void CentralProjectionCamera::turnLeft(qreal angle) {
    QVector3D left = -QVector3D::crossProduct(vpn_, vup_);
    vpn_ = cos(angle) * vpn_ + sin(angle) * left;
}

void CentralProjectionCamera::buildTransform() {
    Q_ASSERT(canvas());

    QVector3D rz = -vpn_.normalized();
    QVector3D rx = QVector3D::crossProduct(vpn_, vup_).normalized();
    QVector3D ry = QVector3D::crossProduct(rx, rz).normalized();

    qreal halfWidth = canvas()->width() / 200.0;
    qreal halfHeight = canvas()->height() / 200.0;
    // qDebug() << "halfWidth" << halfWidth;
    // qDebug() << "halfHeight" << halfHeight;

    setTransform(Transform::doNothing()
            .combine(Transform::shift(-cop_))
            .combine(Transform::orthogonal(rx, ry, rz))
            .combine(Transform::reflect(2))
            //.combine(Transform::reflect(0))
            .combine(Transform::scale(near_ / halfWidth, near_ / halfHeight, 1))
            .combine(Transform::scale(1 / far_, 1 / far_, 1 / far_))
            // at this point visible space is cut pyramide
            // -z <= x <= z
            // -z <= y <= z
            // near_ / far_ <= z <= 1
            .combine(Transform::perspective(near_ / far_))
            .combine(Transform::shift(QVector3D(1, 1, 0)))
            .combine(Transform::scale(0.5, 0.5, 1))
            // at this point visible space is a cube
            .combine(Transform::scale(canvas()->width(), canvas()->height(), 1))
            // the last transformation fits to the screen size
            );
}

QVector3D CentralProjectionCamera::position() {
    return cop_;
}

qreal CentralProjectionCamera::height() {
    return cop_.y();
}

qreal CentralProjectionCamera::latitude() {
    return cop_.z();
}

qreal CentralProjectionCamera::longitude() {
    return cop_.x();
}

qreal CentralProjectionCamera::zenith() {
    return atan(vpn_.y() / QVector3D(vpn_.x(), vpn_.z(), 0).length());
}

qreal CentralProjectionCamera::azimuth() {
    return atan2(vpn_.x(), vpn_.z());
}
