#-------------------------------------------------
#
# Project created by QtCreator 2015-07-17T16:09:18
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += network
TARGET = DroneSystem
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app


SOURCES += main.cpp \
    pcsd.cpp \
    ../Utils/Utils/utils.cpp

HEADERS += \
    pcsd.h \
    ../Utils/Utils/utils.h
