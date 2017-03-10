#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T00:53:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BuslineQuery
TEMPLATE = app


SOURCES += main.cpp\
    busline.cpp \
    mainwindow.cpp \
    firstTabDataManager.cpp \
    buslineInfoDialog.cpp \
    thirdTabDataManager.cpp \
    buslinedetailview.cpp \
    queryresultview.cpp

HEADERS  += \
    busline.h \
    mainwindow.h \
    firstTabDataManager.h \
    buslineInfoDialog.h \
    thirdTabDataManager.h \
    buslinedetailview.h \
    queryresultview.h

FORMS    += \
    mainwindow.ui \
    buslineInfoDialog.ui \
    buslinedetailview.ui \
    queryresultview.ui

DISTFILES += \
    data.txt \
    busstops.txt
