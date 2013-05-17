#ifndef LAB_H
#define LAB_H

#include <QTimer>

#include "scenario.h"

class Lab : public Scenario
{
    Q_OBJECT
public:
    explicit Lab(QMainWindow * mainWindow);

    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void timerTimeout();

private:
    QTimer timer;
    qint64 delta;
    qint64 now;
    bool stop;
};

#endif // LAB_H
