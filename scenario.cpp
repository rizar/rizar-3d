#include "scenario.h"
#include "camera.h"

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

#include <cmath>

Scenario::Scenario(QMainWindow * mainWindow) :
    QObject(mainWindow),
    camera_(new CentralProjectionCamera)
{
}

Scenario::~Scenario()
{
}

QMainWindow * Scenario::mainWindow() {
    return dynamic_cast<QMainWindow *>(parent());
}

ICamera * Scenario::camera() {
    return camera_;
}

void Scenario::keyPressEvent(QKeyEvent *event)
{
    qreal const STEP = 1;
    qreal const ANGLE = acos(-1.0) / 2 / 45;
    if (event->key() == Qt::Key_W) {
        camera()->moveForward(STEP);
    }
    if (event->key() == Qt::Key_S) {
        camera()->moveBackward(STEP);
    }
    if (event->key() == Qt::Key_A) {
        camera()->moveLeft(STEP);
    }
    if (event->key() == Qt::Key_D) {
        camera()->moveRight(STEP);
    }
    if (event->key() == Qt::Key_I) {
        camera()->turnUp(ANGLE);
    }
    if (event->key() == Qt::Key_K) {
        camera()->turnDown(ANGLE);
    }
    if (event->key() == Qt::Key_J) {
        camera()->turnLeft(ANGLE);
    }
    if (event->key() == Qt::Key_L) {
        camera()->turnRight(ANGLE);
    }
    mainWindow()->repaint();
}

void Scenario::textCell(QPainter &painter, quint32 row, quint32 column, const QString &text) {
    painter.drawText(QRect(mainWindow()->width() - 100 * (row + 1),
                           mainWindow()->height() - 40 * (column + 1),
                           mainWindow()->width() - 100 * row,
                           mainWindow()->height() - 40 * column),
                     text);
}

void Scenario::writeCameraState(QPainter & painter) {
    QString buffer;
    QTextStream stream(&buffer);
    stream << fixed << qSetRealNumberPrecision(2);

    stream << "azimuth: " << camera()->azimuth();
    textCell(painter, 0, 0, buffer);
    buffer.clear();

    stream << "zenith: " << camera()->zenith();
    textCell(painter, 0, 1, buffer);
    buffer.clear();

    stream << "latitude: " << camera()->latitude();
    textCell(painter, 0, 2, buffer);
    buffer.clear();

    stream << "longitude: " << camera()->longitude();
    textCell(painter, 0, 3, buffer);
    buffer.clear();

    stream << "height: " << camera()->height();
    textCell(painter, 0, 4, buffer);
    buffer.clear();
}


DefaultScenario::DefaultScenario(QMainWindow * parent) :
    Scenario(parent)
{
}

void DefaultScenario::paintEvent(QPaintEvent *) {
    QPainter painter(mainWindow());
    painter.drawText(mainWindow()->rect(), Qt::AlignCenter, "Choose scenario, please");
}

