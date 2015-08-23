#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T20:05:55
#
#-------------------------------------------------

QT  += core
QT  += gui
QT += opengl
QT += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -L"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x86" -lGLU32 -lglut -lOpenGL32

TARGET = Untitled
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    cubes.cpp

HEADERS  += widget.h \
    cubes.h
