#ifndef BODIES_H
#define BODIES_H

#include "plane.h"

#include <QVector>
#include <QSharedPointer>
#include <QPoint>
#include <QVector3D>
#include <QColor>


class Transform;
class ICanvas;

class IBody {
public:
    virtual ~IBody() {
    }

    virtual IBody * clone() const = 0;
    virtual IBody * transform(Transform const& tr) const = 0;

    virtual void draw(ICanvas * canvas) = 0;
};

class FlatBody : public IBody {
public:
    virtual FlatBody * clone() const = 0;
    virtual FlatBody * transform(Transform const& tr) const = 0;

    virtual Plane getPlane() = 0;

protected:
    QPoint project(QVector3D const& point);
    QPointF projectF(QVector3D const& point);

};

class CompositeBody : public IBody {
public:
    virtual CompositeBody * clone() const;
    virtual CompositeBody * transform(Transform const& tr) const;

    CompositeBody(QVector< QSharedPointer<IBody> > const& parts);
    CompositeBody(CompositeBody const& other);

    quint32 numParts() const;
    IBody * part(quint32 index);

    virtual void draw(ICanvas * canvas);

private:
    QVector< QSharedPointer<IBody> > parts_;
};

class FlatContour : public FlatBody {
public:
    virtual FlatContour * clone() const = 0;
    virtual FlatContour * transform(Transform const& tr) const = 0;

    QColor color() const;
    void setColor(QColor color);

    virtual void rasterize(ICanvas * canvas, QVector<QPoint> * pixels) = 0;
    virtual void draw(ICanvas * canvas);

private:
    QColor color_;
};

class FlatClosedContour : public FlatContour {
public:
    virtual FlatClosedContour * clone() const = 0;
    virtual FlatClosedContour * transform(Transform const& tr) const = 0;

    virtual bool projectionContains(QPoint point) = 0;
};

class FlatPolyline : public FlatContour {
public:
    virtual FlatPolyline * clone() const;
    virtual FlatPolyline * transform(Transform const& tr) const;

    FlatPolyline(QVector<QVector3D> const& points);

    quint32 numPoints();
    QVector3D point(quint32 index);
    QVector3D first();
    QVector3D last();

    virtual Plane getPlane();
    virtual void rasterize(ICanvas * canvas, QVector<QPoint> * pixels);

private:
    QVector<QVector3D> points_;
};

class FlatPolygon : public FlatClosedContour {
public:
    virtual FlatPolygon * clone() const;
    virtual FlatPolygon * transform(Transform const& tr) const;

    FlatPolygon(QVector<QVector3D> const& points);
    FlatPolygon(FlatPolyline const& polyline);
    FlatPolygon(FlatPolygon const& other);

    quint32 numPoints();
    QVector3D point(quint32 index);

    virtual Plane getPlane();
    virtual bool projectionContains(QPoint point);

    virtual void rasterize(ICanvas * canvas, QVector<QPoint> * pixels);

private:
    QScopedPointer<FlatPolyline> polyline_;
};

class FlatArea : public FlatBody {
public:
    virtual FlatArea * clone() const;
    virtual FlatArea * transform(Transform const& tr) const;

    FlatArea(FlatArea const& other);
    FlatArea(FlatClosedContour const& border);

    QColor borderColor() const;
    void setBorderColor(QColor color);

    QColor areaColor() const;
    void setAreaColor(QColor color);

    virtual Plane getPlane();
    virtual void draw(ICanvas *);

private:
    QColor areaColor_;
    QScopedPointer<FlatClosedContour> border_;
};

class Cube : public CompositeBody {
public:
    Cube();
};

class Axes : public CompositeBody {
public:
    Axes();
};

#endif // BODIES_H
