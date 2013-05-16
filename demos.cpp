#include "demos.h"
#include "canvas.h"
#include "bodies.h"
#include "camera.h"

#include <QMainWindow>
#include <QPainter>
#include <QVector3D>
#include <QScopedPointer>
#include <QElapsedTimer>
#include <QDebug>

Demo1::Demo1(QMainWindow * mainWindow) :
    Scenario(mainWindow)
{
}

void Demo1::paintEvent(QPaintEvent *) {
    QPainter painter(mainWindow());
    painter.drawText(mainWindow()->rect(), Qt::AlignCenter, "Demo 1");
}

Demo2::Demo2(QMainWindow * mainWindow) :
    Scenario(mainWindow)
{
}

void Demo2::paintEvent(QPaintEvent *) {
    QPainter painter(mainWindow());
    painter.drawText(mainWindow()->rect(), Qt::AlignCenter, "Demo 2");

    SimpleCanvas canvas(mainWindow());
    FlatPolyline polyline({QVector3D(100, 100, 100),
                           QVector3D(100, 200, 100),
                          QVector3D(200, 100, 100)});
    polyline.draw(&canvas);

    FlatPolygon polygon({QVector3D(300, 300, 100),
                         QVector3D(300, 200, 100),
                         QVector3D(200, 300, 100)});
    polygon.setColor(QColor("red"));
    polygon.draw(&canvas);

    FlatArea smallRectangle(FlatPolygon({QVector3D(10, 10, 10),
                                         QVector3D(90, 10, 10),
                                         QVector3D(90, 90, 10),
                                         QVector3D(10, 90, 10)}));
    smallRectangle.setAreaColor(QColor("blue"));
    smallRectangle.draw(&canvas);

    FlatArea triangle(FlatPolygon({QVector3D(50, 50, 20),
                                   QVector3D(120, 50, 20),
                                   QVector3D(50, 120, 20)}));
    triangle.setAreaColor(QColor("green"));
    triangle.draw(&canvas);

    FlatArea triangle2(FlatPolygon({QVector3D(200, 200, 20),
                                   QVector3D(200, 250, 20),
                                   QVector3D(300, 200, 20)}));
    triangle2.setAreaColor(QColor("green"));
    triangle2.draw(&canvas);
}



Demo3::Demo3(QMainWindow * mainWindow) :
    Scenario(mainWindow)
{
}

void Demo3::paintEvent(QPaintEvent *) {
    QElapsedTimer timer;
    timer.start();

    ZBufferCanvas canvas(mainWindow());

    FlatArea triangle(FlatPolygon({QVector3D(0, 0, 20),
                                   QVector3D(0, 5, 20),
                                   QVector3D(5, 0, 20)}));
    triangle.setAreaColor(QColor("green"));

    FlatArea triangle2(FlatPolygon({QVector3D(0, 4, 19),
                                   QVector3D(0, 19, 19),
                                   QVector3D(5, 4, 19)}));
    triangle2.setAreaColor(QColor("red"));


    QScopedPointer<IBody> cube(
                    QScopedPointer<IBody>(new Cube)->transform(
                        Transform::scale(10, 10, 10).combine(
                        Transform::shift(QVector3D(0, 0, 10))))
                    );

    QScopedPointer<IBody> axes(
                    QScopedPointer<IBody>(new Axes)->transform(
                        Transform::shift(QVector3D(0, 0, 0.1)))
                    );

    camera()->setCanvas(&canvas);
    camera()->draw(*cube);
    //camera()->draw(*axes);
    canvas.flush();

    QPainter painter(mainWindow());
    writeCameraState(painter);
    textCell(painter, 0, 5, QString::number(timer.elapsed() / 1e3, 'f', 3) + " seconds");
}

