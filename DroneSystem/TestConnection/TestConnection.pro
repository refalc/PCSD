#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T23:42:55
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += network
TARGET = TestConnection
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app


SOURCES += main.cpp \
    ../Utils/Utils/utils.cpp \
    testconnection.cpp \
    ../Utils/Utils/cnetworkconnectionfactory.cpp \
    ../Utils/Utils/networkconnection.cpp \
    ../Utils/Utils/networkobject.cpp

HEADERS += \
    ../Utils/Utils/utils.h \
    testconnection.h \
    ../Utils/Utils/cnetworkconnectionfactory.h \
    ../Utils/Utils/networkconnection.h \
    ../Utils/Utils/networkobject.h
