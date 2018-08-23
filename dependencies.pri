QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#External libs

#Tdp libs
include(../tdp_qt_utils/dependencies.pri)

!equals(TARGET, "tdp_qt_widgets"){
LIBS += -ltdp_qt_widgets
}

INCLUDEPATH  += $$PWD/../tdp_qt_widgets/inc/
