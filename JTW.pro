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
    setupform.cpp \
    settingsform.cpp \
    infoform.cpp \
    restartdialog.cpp \
    pwddialog.cpp \
    plccontrolthread.cpp \
    analyzedialog.cpp \
    sampledialog.cpp \
    helpdialog.cpp \
    exitdialog.cpp \
    licensedialog.cpp \
    reportdialog.cpp \
    ../_Modules/Protect/protect.cpp \
    ../_Modules/S7/s7.cpp \
    ../_Modules/Imgprocess/imgprocess.cpp \
    ../_Modules/Getsystemtime/getsystemtime.cpp \
    ../_Modules/Getimage/getImage.cpp

HEADERS  += mainwindow.h \
    setupform.h \
    settingsform.h \
    messages.h \
    infoform.h \
    restartdialog.h \
    pwddialog.h \
    plccontrolthread.h \
    analyzedialog.h \
    sampledialog.h \
    helpdialog.h \
    exitdialog.h \
    licensedialog.h \
    reportdialog.h \
    ../_Modules/Protect/protect.h \
    ../_Modules/S7/s7types.h \
    ../_Modules/S7/s7.h \
    ../_Modules/Imgprocess/imgprocess.h \
    ../_Modules/Imgprocess/imgprocess_msg.h \
    ../_Modules/Getsystemtime/getsystemtime.h \
    ../_Modules/Getimage/getImage.h

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
    libnodave.dll \
    appicon.rc

win32:RC_FILE = appicon.rc
