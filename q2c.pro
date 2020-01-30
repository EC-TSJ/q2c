#-------------------------------------------------
#
# Project created by QtCreator 2013-11-26T17:11:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = q2c
CONFIG   += console c++11
CONFIG   -= app_bundle

win32:RC_FILE += main.rc

TEMPLATE = app


SOURCES += main.cpp \
    CommandParser.cpp \
    Generic.cpp \
    project.cpp \
    logs.cpp

HEADERS += \
    CommandParser.h \
    Generic.h \
    _defs.h \
    main.h \
    project.h \
    logs.h
