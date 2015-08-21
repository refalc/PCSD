#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T20:05:55
#
#-------------------------------------------------

QT  += core
QT  += gui
QT  += opengl
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h
LIBS += -L/usr/local/lib -lGLU
