#-------------------------------------------------
#
# Project created by QtCreator 2010-09-25T00:38:01
#
#-------------------------------------------------

QT       += core gui
QT       += core network

TARGET = JTW
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    getImage.cpp \
    imgprocess.cpp \
    setupform.cpp \
    settingsform.cpp \
    s7.cpp \
    infoform.cpp \
    playthread.cpp \
    getsystemtime.cpp \
    restartdialog.cpp \
    pwddialog.cpp \
    plccheckthread.cpp \
    imggetthread.cpp \
    plccontrolthread.cpp \
    analyzedialog.cpp \
    sampledialog.cpp \
    helpdialog.cpp \
    exitdialog.cpp \
    licensedialog.cpp \
    reportdialog.cpp \
    ../_Modules/Protect/protect.cpp

HEADERS  += mainwindow.h \
    getImage.h \
    imgprocess.h \
    imgprocess.h \
    setupform.h \
    settingsform.h \
    messages.h \
    s7.h \
    s7types.h \
    infoform.h \
    playthread.h \
    getsystemtime.h \
    restartdialog.h \
    pwddialog.h \
    plccheckthread.h \
    imggetthread.h \
    plccontrolthread.h \
    analyzedialog.h \
    sampledialog.h \
    helpdialog.h \
    exitdialog.h \
    licensedialog.h \
    reportdialog.h \
    ../_Modules/Protect/protect.h

FORMS    += mainwindow.ui \
    setupform.ui \
    settingsform.ui \
    infoform.ui \
    restartdialog.ui \
    pwddialog.ui \
    analyzedialog.ui \
    sampledialog.ui \
    helpdialog.ui \
    exitdialog.ui \
    licensedialog.ui \
    reportdialog.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    libnodave.dll
