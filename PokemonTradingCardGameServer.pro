#-------------------------------------------------
#
# Project created by QtCreator 2018-06-22T11:15:48
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokemonTradingCardGameServer
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

Debug:OBJECTS_DIR = debug/obj/
Debug:MOC_DIR = debug/moc/

Release:OBJECTS_DIR = release/obj/
Release:MOC_DIR = release/moc/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tcpserverclients.cpp \
    threadclient.cpp \
    instancemanager.cpp \
    authentification.cpp \
    Share/constantesshared.cpp \
    Models/modeltablegames.cpp \
    Models/modellistusers.cpp \
    CommunicationInstance/tcpserverinstance.cpp \
    CommunicationInstance/threadinstance.cpp

HEADERS += \
        mainwindow.h \
    tcpserverclients.h \
    threadclient.h \
    instancemanager.h \
    authentification.h \
    Share/constantesshared.h \
    Models/modeltablegames.h \
    Models/modellistusers.h \
    CommunicationInstance/tcpserverinstance.h \
    CommunicationInstance/threadinstance.h

FORMS += \
        mainwindow.ui
