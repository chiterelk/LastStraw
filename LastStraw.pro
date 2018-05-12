#-------------------------------------------------
#
# Project created by QtCreator 2018-02-14T10:20:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LastStraw
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    jlaststraw.cpp \
    JBittrex/jbalance.cpp \
    JBittrex/jmarket.cpp \
    JBittrex/jtickers.cpp \
    jbittrex.cpp \
    JBittrex/jcurrency.cpp \
    JBittrex/jopenedorder.cpp \
    login.cpp \
    JLastStraw/jworkmarket.cpp \
    exceptionslist.cpp

HEADERS += \
        mainwindow.h \
    jlaststraw.h \
    JBittrex/jbalance.h \
    JBittrex/jmarket.h \
    JBittrex/jtickers.h \
    jbittrex.h \
    JBittrex/jcurrency.h \
    JBittrex/jopenedorder.h \
    login.h \
    JLastStraw/jworkmarket.h \
    exceptionslist.h

FORMS += \
        mainwindow.ui \
    login.ui \
    exceptionslist.ui
