#-------------------------------------------------
#
# Project created by QtCreator 2010-09-25T00:38:01
# -----2017-----
#-------------------------------------------------

#QT         += core network
#QT         += core gui multimedia multimediawidgets
#QTPLUGIN   += libqjpg\            libqtiff
#QT         += network widgets

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT       += widgets

TARGET      = JTW
TEMPLATE    = app

DEFINES     += QT_DISABLE_DEPRECATED_BEFORE=0x040900

#CONFIG += staticlib
win32{
    DEFINES += BCCWIN DAVE_LITTLE_ENDIAN
}
unix{
    DEFINES += LINUX DAVE_LITTLE_ENDIAN
}

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
    ../_Modules/Getimage/getImage.cpp \
    videosavethread.cpp \
    imgtools.cpp \
    plcqtlib.cpp \
    nodave.c

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
    ../_Modules/Getimage/getImage.h \
    ../_Modules/Algo/datatypes.h \
    ../_Modules/Algo/localMinimum.h \
    videosavethread.h \
    imgtools.h \
    log2.h \
    nodave.h \
    plcqtlib.h

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

DISTFILES +=

win32: LIBS += -L$$PWD/../_Modules/qtPLC_lib-master/release/ -lplcQtLib -lws2_32

INCLUDEPATH += $$PWD/../_Modules/qtPLC_lib-master/release
DEPENDPATH += $$PWD/../_Modules/qtPLC_lib-master/release

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../_Modules/qtPLC_lib-master/release/plcQtLib.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../_Modules/qtPLC_lib-master/release/libplcQtLib.a
