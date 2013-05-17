#ifndef TESTPOLYLINE_H
#define TESTPOLYLINE_H

#include <QObject>

class TestPolyline : public QObject
{
    Q_OBJECT
public:
    explicit TestPolyline(QObject *parent = 0);
    
private slots:
    void testDrawTriangle();
    void testZCutSquare();
    void testZCutSquare2();
};

#endif // TESTPOLYLINE_H
