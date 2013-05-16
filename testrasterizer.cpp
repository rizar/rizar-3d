#include "testrasterizer.h"
#include "rasterizer.h"

#include <QtTest/QtTest>
#include <QDebug>

TestRasterizer::TestRasterizer(QObject *parent) :
    QObject(parent),
    largeCanvas_(10, 10),
    smallCanvas_(2, 2),
    largeRasterizer_(&largeCanvas_),
    smallRasterizer_(&smallCanvas_)
{
}

void TestRasterizer::testVerticalFit() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(2, 2), QPoint(2, 4), &pixels);
    QVector<QPoint> expected = {QPoint(2, 2), QPoint(2, 3), QPoint(2, 4)};
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testVerticalLeft() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(-1, 2), QPoint(-1, 4), &pixels);
    QVector<QPoint> expected;
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testVerticalRight() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(11, 2), QPoint(11, 4), &pixels);
    QVector<QPoint> expected;
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testVerticalCross() {
    QVector<QPoint> pixels;
    smallRasterizer_.rasterizeLine(QPoint(1, -10000000), QPoint(1, 10000000), &pixels);
    QVector<QPoint> expected = {QPoint(1, 0), QPoint(1, 1)};
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testHorizontalCross() {
    QVector<QPoint> pixels;
    smallRasterizer_.rasterizeLine(QPoint(-10000000, 1), QPoint(10000000, 1), &pixels);
    QVector<QPoint> expected = {QPoint(0, 1), QPoint(1, 1)};
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testHorizontalDown() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(2, -1), QPoint(2, -1), &pixels);
    QVector<QPoint> expected;
    QCOMPARE(pixels, expected);

}

void TestRasterizer::testHorizontalUp() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(2, 11), QPoint(4, 11), &pixels);
    QVector<QPoint> expected;
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testDiagonal() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(-1, -1), QPoint(2, 2), &pixels);
    QVector<QPoint> expected = {QPoint(0, 0), QPoint(1, 1), QPoint(2, 2)};
    QCOMPARE(pixels, expected);
}

void TestRasterizer::testBackwardDiagonal() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(-1, 11), QPoint(2, 8), &pixels);
    QVector<QPoint> expected = {QPoint(1, 9), QPoint(2, 8)};
    QCOMPARE(pixels, expected);
}

void TestRasterizer::textComplexLine1() {
    QVector<QPoint> pixels;
    largeRasterizer_.rasterizeLine(QPoint(1, 1), QPoint(6, 3), &pixels);
    QVector<QPoint> expected = {QPoint(1,1), QPoint(2,1), QPoint(3,2), QPoint(4,2), QPoint(5,3), QPoint(6,3)};
    QCOMPARE(pixels, expected);
}

