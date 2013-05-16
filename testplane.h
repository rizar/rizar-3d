#ifndef TESTPLANE_H
#define TESTPLANE_H

#include <QObject>

class TestPlane : public QObject
{
    Q_OBJECT
public:
    explicit TestPlane(QObject *parent = 0);

private slots:
    void testHorizontalPlane();
    void testPlaneThroughThreePoints();
    void testPlaneThroughTwoPoints();
};

#endif // TESTPLANE_H
