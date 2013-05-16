#include "testpolygon.h"
#include "bodies.h"

#include <QtTest/QTest>

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
