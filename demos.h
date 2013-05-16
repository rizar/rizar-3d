#ifndef DEMOS_H
#define DEMOS_H

#include "scenario.h"

#include <QObject>

class QMainWindow;
class QPaintEvent;

class Demo1 : public Scenario {
    Q_OBJECT
public:
    explicit Demo1(QMainWindow * mainWindow = 0);

    void paintEvent(QPaintEvent * event);
};

class Demo2 : public Scenario {
    Q_OBJECT
public:
    explicit Demo2(QMainWindow * mainWindow = 0);

    void paintEvent(QPaintEvent * event);
};

class Demo3 : public Scenario {
    Q_OBJECT
public:
    explicit Demo3(QMainWindow * mainWindow = 0);

    void paintEvent(QPaintEvent * event);
};

#endif // DEMOS_H
