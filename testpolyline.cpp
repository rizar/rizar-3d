#include "testpolyline.h"
#include "bodies.h"

#include <QtTest/QtTest>
#include <QDebug>

TestPolyline::TestPolyline(QObject *parent) :
    QObject(parent)
{
}

void TestPolyline::testDrawTriangle() {
}

void TestPolyline::testZCutSquare() {
    FlatPolyline polyline({QVector3D(0, 3, 5),
                           QVector3D(0, -2, 5),
                           QVector3D(0, -2, 1),
                           QVector3D(0, 3, 1)});
    QScopedPointer<FlatPolyline> cut(polyline.zCut(2));
    QCOMPARE((qint32)cut->numPoints(), 3);
    QCOMPARE(cut->point(0), QVector3D(0, 3, 5));
    QCOMPARE(cut->point(1), QVector3D(0, -2, 5));
    QCOMPARE(cut->point(2), QVector3D(0, -2, 2));
}

void TestPolyline::testZCutSquare2() {
    FlatPolyline polyline({QVector3D(0, 3, 1),
                           QVector3D(0, 3, 5),
                           QVector3D(0, -2, 5),
                           QVector3D(0, -2, 1)});
    QScopedPointer<FlatPolyline> cut(polyline.zCut(2));
    QCOMPARE((qint32)cut->numPoints(), 4);
    QCOMPARE(cut->point(0), QVector3D(0, 3, 2));
    QCOMPARE(cut->point(1), QVector3D(0, 3, 5));
    QCOMPARE(cut->point(2), QVector3D(0, -2, 5));
    QCOMPARE(cut->point(3), QVector3D(0, -2, 2));
}
