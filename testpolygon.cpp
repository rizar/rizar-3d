#include "testpolygon.h"
#include "bodies.h"

#include <QtTest/QTest>
#include <QDebug>

TestPolygon::TestPolygon(QObject *parent) :
    QObject(parent)
{
}

void TestPolygon::testProjectionContains() {
    FlatPolygon polygon({QVector3D(0, 5, 1),
                         QVector3D(5, 0, 1),
                         QVector3D(0, 0, 1)});
    QCOMPARE(polygon.projectionContains(QPoint(1, 1)), true);
    QCOMPARE(polygon.projectionContains(QPoint(-1, 1)), false);
    QCOMPARE(polygon.projectionContains(QPoint(1, -1)), false);
    QCOMPARE(polygon.projectionContains(QPoint(5, 5)), false);
}

void TestPolygon::testZCutPolygon()
{
     FlatPolygon polygon({QVector3D(0, 3, 5),
                          QVector3D(0, -2, 5),
                          QVector3D(0, -2, 1),
                          QVector3D(0, 3, 1)});
     QScopedPointer<FlatPolygon> cut(polygon.zCut(2));
     QCOMPARE((qint32)cut->numPoints(), 4);
     QCOMPARE(cut->point(0), QVector3D(0, 3, 5));
     QCOMPARE(cut->point(1), QVector3D(0, -2, 5));
     QCOMPARE(cut->point(2), QVector3D(0, -2, 2));
     QCOMPARE(cut->point(3), QVector3D(0, 3, 2));
}
