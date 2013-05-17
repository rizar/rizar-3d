#include <QtTest/QtTest>

#include "transform.h"
#include "testtransform.h"

TestTransform::TestTransform(QObject *parent) :
    QObject(parent),
    ox_(1, 0, 0),
    oy_(0, 1, 0),
    oz_(0, 0, 1)
{
}

void TestTransform::doNothingApply() {
    QVERIFY(qFuzzyCompare(Transform::doNothing().apply(ox_), ox_));
    QVERIFY(qFuzzyCompare(Transform::doNothing().apply(oy_), oy_));
    QVERIFY(qFuzzyCompare(Transform::doNothing().apply(oz_), oz_));
}

void TestTransform::shiftApply() {
    QVector3D onVector(1, 2, 3);
    QVERIFY(qFuzzyCompare(Transform::shift(onVector).apply(ox_), ox_ + onVector));
    QVERIFY(qFuzzyCompare(Transform::shift(onVector).apply(oy_), oy_ + onVector));
    QVERIFY(qFuzzyCompare(Transform::shift(onVector).apply(oz_), oz_ + onVector));
}

void TestTransform::reflectApply() {
    QVERIFY(qFuzzyCompare(Transform::reflect(0).apply(ox_), -ox_));
    QVERIFY(qFuzzyCompare(Transform::reflect(1).apply(oy_), -oy_));
    QVERIFY(qFuzzyCompare(Transform::reflect(2).apply(oz_), -oz_));
}

void TestTransform::orthogonalApply()
{
    Transform transform = Transform::orthogonal(QVector3D(0, -1, 0), QVector3D(1, 0, 0), QVector3D(0, 0, 1));
    QCOMPARE(transform.apply(QVector3D(1, 1, 0)), QVector3D(-1, 1, 0));
}

void TestTransform::rotateXYApply()
{
    Transform transform = Transform::rotateXY(acos(0));
    QCOMPARE(transform.apply(QVector3D(1, 1, 0)), QVector3D(-1, 1, 0));
}

void TestTransform::rotateXZApply()
{
    Transform transform = Transform::rotateXZ(acos(0));
    QCOMPARE(transform.apply(QVector3D(1, 0, 1)), QVector3D(-1, 0, 1));
}

void TestTransform::rotateYZApply()
{
    Transform transform = Transform::rotateYZ(acos(0));
    QCOMPARE(transform.apply(QVector3D(0, 1, 1)), QVector3D(0, -1, 1));
}

void TestTransform::scaleApply()
{
    QCOMPARE(Transform::scale(2, 3, 4).apply(QVector3D(3, 5, 6)), QVector3D(6, 15, 24));
}
