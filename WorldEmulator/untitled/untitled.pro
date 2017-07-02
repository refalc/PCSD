QT  += core
QT  += gui
QT += opengl
QT += network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -L"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x86" -lGLU32 -lglut -lOpenGL32

TARGET = Untitled
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    cubes.cpp

HEADERS  += widget.h \
    cubes.h
