#-------------------------------------------------
#
# Project created by QtCreator 2013-04-22T14:20:45
#
#-------------------------------------------------

QT       += core gui

TARGET = lab
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    transform.cpp \
    testtransform.cpp \
    canvas.cpp \
    scenario.cpp \
    demos.cpp \
    rasterizer.cpp \
    testrasterizer.cpp \
    plane.cpp \
    testpolyline.cpp \
    testplane.cpp \
    bodies.cpp \
    testpolygon.cpp \
    camera.cpp \
    lab.cpp

HEADERS  += mainwindow.h \
    transform.h \
    testtransform.h \
    canvas.h \
    scenario.h \
    demos.h \
    rasterizer.h \
    testrasterizer.h \
    bodies.h \
    plane.h \
    testpolyline.h \
    testplane.h \
    testpolygon.h \
    camera.h \
    lab.h

FORMS    += mainwindow.ui

QMAKE_CC = gcc-4.8
QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -gdwarf-2
