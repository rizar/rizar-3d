#include "plane.h"
#include "testplane.h"
#include <QtTest/QtTest>

TestPlane::TestPlane(QObject *parent) :
    QObject(parent)
{
}

void TestPlane::testHorizontalPlane() {
    Plane plane(QVector3D(0, 0, 1), QVector3D(0, 0, 7));
    for (qint32 i = -1; i <= 1; i++) {
        for (qint32 j = -1; j <= 1; j++) {
            QCOMPARE(plane.calculateZ(i, j), 7.0);
        }
    }
}

void TestPlane::testPlaneThroughThreePoints() {
    Plane plane(QVector3D(2, 2, 2), QVector3D(1, 5, 0), QVector3D(-2, -2, 10));
    for (qint32 i = -1; i <= 1; i++) {
        for (qint32 j = -1; j <= 1; j++) {
            QCOMPARE(plane.calculateZ(i, j), static_cast<qreal>(6 - i - j));
        }
    }
}

void TestPlane::testPlaneThroughTwoPoints() {
    Plane plane = Plane::anyPlaneThroughTwoPoints(QVector3D(1, 1, 1), QVector3D(1, 2, 1));
    for (qint32 i = -1; i <= 1; i++) {
        for (qint32 j = -1; j <= 1; j++) {
            QCOMPARE(plane.calculateZ(i, j), static_cast<qreal>(i));
        }
    }
}
