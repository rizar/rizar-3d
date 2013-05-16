#ifndef TESTRASTERIZER_H
#define TESTRASTERIZER_H

#include <QObject>
#include <QScopedPointer>

#include "rasterizer.h"
#include "canvas.h"

class TestRasterizer : public QObject
{
    Q_OBJECT
public:
    explicit TestRasterizer(QObject *parent = 0);
    
private slots:
    void testVerticalFit();
    void testVerticalLeft();
    void testVerticalRight();
    void testVerticalCross();

    void testHorizontalCross();
    void testHorizontalDown();
    void testHorizontalUp();

    void testDiagonal();
    void testBackwardDiagonal();

    void textComplexLine1();

private:
    MocCanvas largeCanvas_;
    MocCanvas smallCanvas_;

    Rasterizer largeRasterizer_;
    Rasterizer smallRasterizer_;
};

#endif // TESTRASTERIZER_H
