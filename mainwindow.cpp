#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scenario.h"
#include "demos.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionDemo1, SIGNAL(triggered()), this, SLOT(demo1()));
    connect(ui->actionDemo2, SIGNAL(triggered()), this, SLOT(demo2()));
    connect(ui->actionDemo3, SIGNAL(triggered()), this, SLOT(demo3()));

    scenario_.reset(new DefaultScenario(this));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::demo1() {
    scenario_.reset(new Demo1(this));
    repaint();
}

void MainWindow::demo2() {
    scenario_.reset(new Demo2(this));
    repaint();
}

void MainWindow::demo3() {
    scenario_.reset(new Demo3(this));
    repaint();
}

void MainWindow::paintEvent(QPaintEvent * event) {
    scenario_->paintEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    scenario_->keyPressEvent(event);
}
