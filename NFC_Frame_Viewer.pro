#-------------------------------------------------
#
# Project created by QtCreator 2014-10-20T11:04:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NFC_Frame_Viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    importmessages.cpp \
    mainsettings.cpp \
    mainwindow_actions.cpp \
    parseLog.cpp \
    parseSwpRing.cpp \
    parsing.cpp \
    parsingact.cpp \
    parsingappcmd.cpp \
    parsingclt.cpp \
    parsingfactory.cpp \
    parsingnci.cpp \
    parsingncicmdnotif.cpp \
    parsingncidata.cpp \
    parsingncidatacmd.cpp \
    parsingoptioncheckbox.cpp \
    parsingrfframe.cpp \
    parsingrfu.cpp \
    parsingshdlc_i.cpp \
    parsingshdlc_s.cpp \
    parsingshdlc_u.cpp \
    parsingspecificdata.cpp \
    parsingswp.cpp \
    searchdialog.cpp

HEADERS  += mainwindow.h \
    importmessages.h \
    mainsettings.h \
    parsermacros.h \
    parsing.h \
    parsingact.h \
    parsingappcmd.h \
    parsingclt.h \
    parsingfactory.h \
    parsingnci.h \
    parsingncicmdnotif.h \
    parsingncidata.h \
    parsingncidatacmd.h \
    parsingoptioncheckbox.h \
    parsingrfframe.h \
    parsingrfu.h \
    parsingshdlc_i.h \
    parsingshdlc_s.h \
    parsingshdlc_u.h \
    parsingspecificdata.h \
    parsingswp.h \
    searchdialog.h \
    typedef.h

FORMS    += mainwindow.ui
