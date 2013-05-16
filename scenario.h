#ifndef SCENARIO_H
#define SCENARIO_H

#include <QObject>

class QMainWindow;
class QPaintEvent;
class QKeyEvent;
class ICamera;

class QPainter;

class Scenario : public QObject {
    Q_OBJECT
public:
    explicit Scenario(QMainWindow * mainWindow = 0);
    virtual ~Scenario();
    
    virtual void paintEvent(QPaintEvent * event) = 0;
    virtual void keyPressEvent(QKeyEvent * event);

    ICamera * camera();
    QMainWindow * mainWindow();

protected:
    void textCell(QPainter & painter, quint32 row, quint32 column, QString const& text);
    void writeCameraState(QPainter & painter);

private:
    ICamera * camera_;
};

class DefaultScenario : public Scenario {
    Q_OBJECT
public:
    explicit DefaultScenario(QMainWindow * mainWindow = 0);

    virtual void paintEvent(QPaintEvent * event);

};


#endif // SCENARIO_H
