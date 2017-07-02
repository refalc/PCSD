QT       += core
QT       -= gui
QT += network
TARGET = DroneSystem
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app

SOURCES += \
    utils.cpp \
    networkobject.cpp \
    networkconnection.cpp \
    cnetworkconnectionfactory.cpp

HEADERS += \
    utils.h \
    networkobject.h \
    networkconnection.h \
    cnetworkconnectionfactory.h
