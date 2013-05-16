#ifndef TESTPOLYGON_H
#define TESTPOLYGON_H

#include <QObject>

class TestPolygon : public QObject
{
    Q_OBJECT
public:
    explicit TestPolygon(QObject *parent = 0);
    
private slots:
    void testProjectionContains();
};

#endif // TESTPOLYGON_H
