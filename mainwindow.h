#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Scenario;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();
    
public slots:
    void demo1();
    void demo2();
    void demo3();

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    Ui::MainWindow *ui;
    QScopedPointer<Scenario> scenario_;
};

#endif // MAINWINDOW_H
