#-------------------------------------------------
#
# Project created by QtCreator 2018-02-25T13:28:41
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = tst_test_corecontroller
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS UNIT_TEST

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        tst_test_corecontroller.cpp \ 
    ../../SysTechTestTask/corecontroller.cpp \
    ../../SysTechTestTask/dbcontroller.cpp \
    ../../SysTechTestTask/employee.cpp \
    ../../SysTechTestTask/employeeconfig.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../SysTechTestTask/corecontroller.h \
    ../../SysTechTestTask/dbcontroller.h \
    ../../SysTechTestTask/employee.h \
    ../../SysTechTestTask/employeeconfig.h
