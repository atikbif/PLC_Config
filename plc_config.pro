#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T14:11:40
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += serialport
QT       += network
QT       += svg



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = logo.ico

TARGET = plc_config
TEMPLATE = app

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += D:/BOOST/boost_mingw_530/include/boost-1_67
LIBS += "-LD:/BOOST/boost_mingw_530/lib" \
        -llibboost_date_time-mgw53-mt-x32-1_67


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    AutoSearch/detectedcontroller.cpp \
    AutoSearch/scancontroller.cpp \
    AutoSearch/scangui.cpp \
    AutoSearch/scanuart.cpp \
    AutoSearch/searchcontroller.cpp \
    Protocols/asciidecorator.cpp \
    Protocols/checksum.cpp \
    Protocols/commanddecorator.cpp \
    Protocols/commandinterface.cpp \
    Protocols/request.cpp \
    Protocols/rk.cpp \
    Protocols/rkcommand.cpp \
    Protocols/udpdecorator.cpp \
    Loader/bootmodesetter.cpp \
    Loader/sfram.cpp \
    Loader/sysframreadwrite.cpp \
    Loader/ymodem.cpp \
    Loader/ymodemthread.cpp \
    Dialogs/dialogabout.cpp \
    Dialogs/dialogdeviceselect.cpp \
    Dialogs/dialognewproject.cpp \
    Dialogs/dialogpurposeselect.cpp \
    ConfigHierarchy/jsonstandardconfigreader.cpp \
    ConfigHierarchy/standardconfigstorage.cpp \
    InputOutput/inputoutput.cpp \
    InputOutput/ioclassification.cpp \
    InputOutput/jsonioclassificationreader.cpp \
    PLC/jsonplcviewreader.cpp \
    PLC/plc.cpp \
    PLC/plcview.cpp \
    PLC/plcviewstorage.cpp \
    FRAM/boilpinframconverter.cpp \
    ConfigHierarchy/jsondevicepatternreader.cpp \
    PLCGUI/plcgui.cpp \
    PLCGUI/pingraphicsrectitem.cpp \
    Dialogs/dialogdiconfig.cpp \
    Dialogs/dialogpressureconfig.cpp \
    Dialogs/dialogtemperatureconfig.cpp \
    Dialogs/dialoglevelconfig.cpp \
    Dialogs/dialogfrequencyconfig.cpp \
    Dialogs/dialogdefaultaiconfig.cpp \
    InputOutput/aiconfig.cpp \
    Dialogs/dialogmainconfig.cpp

HEADERS += \
        mainwindow.h \
    AutoSearch/detectedcontroller.h \
    AutoSearch/scancontroller.h \
    AutoSearch/scangui.h \
    AutoSearch/scanuart.h \
    AutoSearch/searchcontroller.h \
    Protocols/asciidecorator.h \
    Protocols/checksum.h \
    Protocols/commanddecorator.h \
    Protocols/commandinterface.h \
    Protocols/request.h \
    Protocols/rk.h \
    Protocols/rkcommand.h \
    Protocols/udpdecorator.h \
    Loader/bootmodesetter.h \
    Loader/sfram.h \
    Loader/sysframreadwrite.h \
    Loader/ymodem.h \
    Loader/ymodemthread.h \
    Dialogs/dialogabout.h \
    Dialogs/dialogdeviceselect.h \
    Dialogs/dialognewproject.h \
    Dialogs/dialogpurposeselect.h \
    ConfigHierarchy/jsonstandardconfigreader.h \
    ConfigHierarchy/standardconfigreader.h \
    ConfigHierarchy/standardconfigstorage.h \
    InputOutput/iioclassificationreader.h \
    InputOutput/inputoutput.h \
    InputOutput/iobase.h \
    InputOutput/ioclassification.h \
    InputOutput/jsonioclassificationreader.h \
    PLC/ioview.h \
    PLC/iplcviewreader.h \
    PLC/jsonplcviewreader.h \
    PLC/plc.h \
    PLC/plcview.h \
    PLC/plcviewstorage.h \
    FRAM/boilpinframconverter.h \
    FRAM/pinframconverter.h \
    ConfigHierarchy/jsondevicepatternreader.h \
    ConfigHierarchy/devicepatternreader.h \
    PLCGUI/plcgui.h \
    PLCGUI/pingraphicsrectitem.h \
    Dialogs/dialogdiconfig.h \
    Dialogs/dialogpressureconfig.h \
    Dialogs/dialogtemperatureconfig.h \
    Dialogs/dialoglevelconfig.h \
    Dialogs/dialogfrequencyconfig.h \
    Dialogs/dialogdefaultaiconfig.h \
    InputOutput/aiconfig.h \
    Dialogs/dialogmainconfig.h

FORMS += \
        mainwindow.ui \
    AutoSearch/scangui.ui \
    Loader/sysframreadwrite.ui \
    Loader/ymodemthread.ui \
    Dialogs/dialogabout.ui \
    Dialogs/dialogdeviceselect.ui \
    Dialogs/dialognewproject.ui \
    Dialogs/dialogpurposeselect.ui \
    Dialogs/dialogdiconfig.ui \
    Dialogs/dialogpressureconfig.ui \
    Dialogs/dialogtemperatureconfig.ui \
    Dialogs/dialoglevelconfig.ui \
    Dialogs/dialogfrequencyconfig.ui \
    Dialogs/dialogdefaultaiconfig.ui \
    Dialogs/dialogmainconfig.ui

RESOURCES += \
    images.qrc \
    config.qrc \
    flash.qrc

DISTFILES +=
