QT       += core

QT       -= gui
QT += network
TARGET = frequency
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    frequency.cpp \
    ../../Utils/Utils/utils.cpp

HEADERS += \
    frequency.h \
    ../../Utils/Utils/utils.h
