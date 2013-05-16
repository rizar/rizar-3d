#include "bodies.h"
#include "canvas.h"
#include "rasterizer.h"
#include "transform.h"

#include <QDebug>

quint32 CompositeBody::numParts() const {
    return parts_.size();
}

IBody * CompositeBody::part(quint32 index) {
    return parts_[index].data();
}

CompositeBody * CompositeBody::clone() const {
    return new CompositeBody(*this);
}

CompositeBody * CompositeBody::transform(Transform const& tr) const {
    CompositeBody * transformed = clone();
    for (quint32 i = 0; i < numParts(); i++) {
        transformed->parts_[i] =
            QSharedPointer<IBody>(transformed->part(i)->transform(tr));
    }
    return transformed;
}

CompositeBody::CompositeBody(const CompositeBody &other) :
    parts_(other.parts_)
{
}

CompositeBody::CompositeBody(QVector<QSharedPointer<IBody> > const& parts) :
    parts_(parts)
{
}

void CompositeBody::draw(ICanvas *canvas)
{
    for (qint32 i = 0; i < parts_.size(); i++) {
        parts_[i]->draw(canvas);
    }
}

QPoint FlatBody::project(QVector3D const& point3D) {
    return QPoint(qRound(point3D.x()), qRound(point3D.y()));
}

QPointF FlatBody::projectF(QVector3D const& point3D) {
    return QPointF(point3D.x(), point3D.y());
}

QColor FlatContour::color() const {
    return color_;
}

void FlatContour::setColor(QColor color) {
    color_ = color;
}

void FlatContour::draw(ICanvas * canvas) {
   // qDebug() << "FlatContour.draw.enter";
    Plane plane(getPlane());
    if (plane.isSingular()) {
        return;
    }

    QVector<QPoint> pixels;
    rasterize(canvas, &pixels);

    QScopedPointer<IPainter> painter(canvas->createPainter());
    painter->setColor(color());

    for (qint32 i = 0; i < pixels.size(); i++) {
        //qDebug() << pixels[i];
        painter->drawPixel(pixels[i], plane.calculateZ(pixels[i]));
    }
    //qDebug() << "FlatContour.draw.exit";
}

FlatPolyline * FlatPolyline::clone() const {
    return new FlatPolyline(*this);
}

FlatPolyline * FlatPolyline::transform(const Transform &tr) const {
    FlatPolyline * transformed = new FlatPolyline(*this);
    for (qint32 i = 0; i < transformed->points_.size(); i++) {
        transformed->points_[i] = tr.apply(transformed->points_[i]);
    }
    //qDebug() << transformed->points_;
    return transformed;
}

FlatPolyline::FlatPolyline(QVector<QVector3D> const& points) :
    points_(points)
{
}

Plane FlatPolyline::getPlane() {
    return Plane::anyPlaneThroughPoints(points_);
}

void FlatPolyline::rasterize(ICanvas *canvas, QVector<QPoint> *pixels) {
    Rasterizer rasterizer(canvas);
    rasterizer.setClearDestination(false);
    pixels->clear();
    for (qint32 i = 0; i + 1 < points_.size(); i++) {
        rasterizer.rasterizeLine(project(points_[i]), project(points_[i + 1]), pixels);
    }
}

quint32 FlatPolyline::numPoints() {
    return points_.size();
}

QVector3D FlatPolyline::point(quint32 index) {
    return points_.at(index);
}

QVector3D FlatPolyline::first() {
    return point(0);
}

QVector3D FlatPolyline::last() {
    return point(numPoints() - 1);
}

FlatPolygon * FlatPolygon::clone() const {
    return new FlatPolygon(*this);
}

FlatPolygon * FlatPolygon::transform(Transform const& tr) const {
    FlatPolygon * transformed = new FlatPolygon(*this);
    transformed->polyline_.reset(polyline_->transform(tr));
    return transformed;
}

FlatPolygon::FlatPolygon(const QVector<QVector3D> &points) :
    polyline_(new FlatPolyline(points))
{
}


FlatPolygon::FlatPolygon(const FlatPolygon &other)
    : polyline_(other.polyline_->clone())
{
}

FlatPolygon::FlatPolygon(const FlatPolyline &polyline) :
    polyline_(polyline.clone())
{
}

quint32 FlatPolygon::numPoints() {
    return polyline_->numPoints();
}

QVector3D FlatPolygon::point(quint32 index) {
    return polyline_->point(index);
}

Plane FlatPolygon::getPlane() {
    return polyline_->getPlane();
}

bool FlatPolygon::projectionContains(QPoint aPoint) {
    quint32 numPlus = 0;
    quint32 numMinus = 0;

    QPointF pointF(aPoint);

    for (quint32 i = 0; i < numPoints(); i++) {
        QPointF pI(projectF(point(i)));
        QPointF pJ(projectF(point((i + 1) % numPoints())));

        QPointF sideDir(pJ - pI);
        QPointF pointDir(pointF - pI);

        qreal crossProd = sideDir.x() * pointDir.y() - sideDir.y() * pointDir.x();
        if (crossProd > 0) {
            numPlus++;
        }
        else {
            numMinus++;
        }
    }

    return numPlus == numPoints() || numMinus == numPoints();
}

void FlatPolygon::rasterize(ICanvas * canvas, QVector<QPoint> * pixels) {
    polyline_->rasterize(canvas, pixels);
    Rasterizer(canvas)
        .setClearDestination(false)
        .rasterizeLine(project(polyline_->last()), project(polyline_->first()), pixels);
}

FlatArea * FlatArea::clone() const {
    return new FlatArea(*this);
}

FlatArea * FlatArea::transform(const Transform &tr) const
{
    FlatArea * transformed = clone();
    transformed->border_.reset(border_->transform(tr));
    return transformed;
}

FlatArea::FlatArea(const FlatArea &other)
    : areaColor_(other.areaColor())
    , border_(other.border_->clone())
{
}

FlatArea::FlatArea(const FlatClosedContour &border) :
    border_(border.clone())
{
}

QColor FlatArea::areaColor() const {
    return areaColor_;
}

void FlatArea::setAreaColor(QColor color) {
    areaColor_ = color;
}

Plane FlatArea::getPlane() {
    return border_->getPlane();
}

void FlatArea::draw(ICanvas * canvas) {
    Plane plane = getPlane();
    if (plane.isSingular()) {
        return;
    }

    QVector< QVector<qint32> > y2xs(
        canvas->height(),
        QVector<qint32>({-1, (qint32)(canvas->width())}));

    QVector<QPoint> pixels;
    border_->rasterize(canvas, &pixels);

    for (qint32 i = 0; i < pixels.size(); i++) {
        y2xs[pixels[i].y()].push_back(pixels[i].x());
    }
    border_->draw(canvas);

    QScopedPointer<IPainter> painter(canvas->createPainter());
    painter->setColor(areaColor());

    for (quint32 y = 0; y < canvas->height(); y++) {
        qSort(y2xs[y]);
        for (qint32 j = 0; j + 1 < y2xs[y].size(); j++) {
            qint32 x1 = y2xs[y][j];
            qint32 x2 = y2xs[y][j + 1];

            if (x2 - x1 <= 1) {
                continue;
            }

            if (border_->projectionContains(QPoint((x1 + x2) / 2, y))) {
                for (qint32 x = x1 + 1; x < x2; x++) {
                    painter->drawPixel(QPoint(x, y), plane.calculateZ(x, y));
                }
            }
        }
    }
}

Cube::Cube() :
    CompositeBody(QVector< QSharedPointer<IBody> >
                  ({
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(0, 0, 0),
                           QVector3D(1, 0, 0),
                           QVector3D(1, 1, 0),
                           QVector3D(0, 1, 0)
                       }))),
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(0, 0, 1),
                           QVector3D(1, 0, 1),
                           QVector3D(1, 1, 1),
                           QVector3D(0, 1, 1)
                       }))),
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(0, 0, 0),
                           QVector3D(1, 0, 0),
                           QVector3D(1, 0, 1),
                           QVector3D(0, 0, 1)
                       }))),
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(0, 1, 0),
                           QVector3D(1, 1, 0),
                           QVector3D(1, 1, 1),
                           QVector3D(0, 1, 1)
                       }))),
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(0, 0, 0),
                           QVector3D(0, 1, 0),
                           QVector3D(0, 1, 1),
                           QVector3D(0, 0, 1)
                       }))),
                       QSharedPointer<IBody>(new FlatArea(
                       FlatPolygon({
                           QVector3D(1, 0, 0),
                           QVector3D(1, 1, 0),
                           QVector3D(1, 1, 1),
                           QVector3D(1, 0, 1)
                       })))
                  }))
{
    for (quint32 i = 0; i < 6; i++) {
        dynamic_cast<FlatArea*>(part(i))->setAreaColor(QColor(i / 2 == 0 ? 255 : 0,
                                                              i / 2 == 1 ? 255 : 0,
                                                              i / 2 == 2 ? 255 : 0));
    }
}

Axes::Axes() :
    CompositeBody(QVector< QSharedPointer<IBody> >
                  ({
                       QSharedPointer<IBody>(new FlatPolyline({
                           QVector3D(0, 0, 0),
                           QVector3D(1, 0, 0)
                       })),
                       QSharedPointer<IBody>(new FlatPolyline({
                           QVector3D(0, 0, 0),
                           QVector3D(0, 1, 0)
                       })),
                       QSharedPointer<IBody>(new FlatPolyline({
                           QVector3D(0, 0, 0),
                           QVector3D(0, 0, 1)
                       }))
                   }))
{
    for (quint32 i = 0; i < 3; i++) {
        dynamic_cast<FlatContour*>(part(i))->setColor(QColor(i  == 0 ? 255 : 0,
                                                              i  == 1 ? 255 : 0,
                                                              i  == 2 ? 255 : 0));
    }
}
