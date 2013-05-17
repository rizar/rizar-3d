#include "lab.h"
#include "canvas.h"
#include "camera.h"
#include "bodies.h"

#include <QElapsedTimer>
#include <QPainter>
#include <QMainWindow>
#include <QKeyEvent>

Lab::Lab(QMainWindow *mainWindow)
    : Scenario(mainWindow)
    , delta(40)
    , now(0)
{
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    timer.start(delta);
}

void Lab::paintEvent(QPaintEvent * /*event*/) {
    QElapsedTimer timer;
    timer.start();

    ZBufferCanvas canvas(mainWindow());

    QScopedPointer<IBody> cube(
                    QScopedPointer<IBody>(new Cube)->transform(
                        Transform::shift(QVector3D(-0.5, -0.5, -0.5)).combine(
                        Transform::rotateXY(5e-5 * now)).combine(
                        Transform::rotateXZ(1e-4 * now)).combine(
                        Transform::rotateYZ(5e-5 * now)).combine(
                        Transform::shift(QVector3D(0.5, 0.5, 0.5))).combine(
                        Transform::scale(10, 10, 10)).combine(
                        Transform::shift(QVector3D(-5, -5, 25)))
                        )
                    );


    camera()->setCanvas(&canvas);
    camera()->draw(*cube);
    canvas.flush();

    QPainter painter(mainWindow());
    writeCameraState(painter);
    textCell(painter, 0, 5, QString::number(timer.elapsed() / 1e3, 'f', 3) + " seconds");
}

void Lab::keyPressEvent(QKeyEvent *event)
{
    Scenario::keyPressEvent(event);
    if (event->key() == Qt::Key_M) {
        stop = !stop;
    }
}

void Lab::timerTimeout()
{
    if (!stop) {
        now += delta;
        mainWindow()->repaint();
    }
}
