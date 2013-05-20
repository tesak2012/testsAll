#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T10:35:11
#
#-------------------------------------------------

QT       += network testlib

QT       -=gui

TARGET = tst_clientservertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    client.cpp \
    tst_clientservertest.cpp\
    server.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    client.h\
    struct_message.h\
    server.h


