#include <QtGui/QApplication>
#include <QtTest/QtTest>

#include "mainwindow.h"
#include "testtransform.h"
#include "testrasterizer.h"
#include "testpolyline.h"
#include "testplane.h"
#include "testpolygon.h"

int main(int argc, char *argv[]) {
    TestTransform testTransform;
    TestRasterizer testRasterizer;
    TestPolyline testPolyline;
    TestPlane testPlane;
    TestPolygon testPolygon;
    int testResult = QTest::qExec(&testTransform);
    testResult |= QTest::qExec(&testRasterizer);
    testResult |= QTest::qExec(&testPolyline);
    testResult |= QTest::qExec(&testPlane);
    testResult |= QTest::qExec(&testPolygon);
    if (testResult != 0) {
        return testResult;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

