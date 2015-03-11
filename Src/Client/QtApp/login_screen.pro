#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T15:35:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = login_screen
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    userinfo.cpp \
    signup.cpp \
    forgotpassword.cpp \
    clientFolder.cpp \
    clientm.cpp

HEADERS  += mainwindow.h \
    userinfo.h \
    signup.h \
    forgotpassword.h \
    clientm.h \
    clientFolder.h

FORMS    += mainwindow.ui \
    userinfo.ui \
    signup.ui \
    forgotpassword.ui


LIBS +=  -L/usr/lib -lssl -lcrypto

OTHER_FILES += \
    b.txt \
    mycert.pem \
    table.txt
