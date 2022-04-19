TARGET = tp_qt_widgets
TEMPLATE = lib

DEFINES += TP_QT_WIDGETS_LIBRARY

SOURCES += src/Utils.cpp
HEADERS += inc/tp_qt_widgets/Utils.h

SOURCES += src/FontUtils.cpp
HEADERS += inc/tp_qt_widgets/FontUtils.h

SOURCES += src/PainterUtils.cpp
HEADERS += inc/tp_qt_widgets/PainterUtils.h

SOURCES += src/RefCountWidget.cpp
HEADERS += inc/tp_qt_widgets/RefCountWidget.h

SOURCES += src/ProgressBar.cpp
HEADERS += inc/tp_qt_widgets/ProgressBar.h

HEADERS += inc/tp_qt_widgets/BlockingOperationDialog.h
SOURCES += src/BlockingOperationDialog.cpp

SOURCES += src/FileDialogLineEdit.cpp
HEADERS += inc/tp_qt_widgets/FileDialogLineEdit.h

SOURCES += src/ColorPickerWidget.cpp
HEADERS += inc/tp_qt_widgets/ColorPickerWidget.h

SOURCES += src/ColorPickerDialog.cpp
HEADERS += inc/tp_qt_widgets/ColorPickerDialog.h

SOURCES += src/DebugWidget.cpp
HEADERS += inc/tp_qt_widgets/DebugWidget.h

SOURCES += src/Globals.cpp
HEADERS += inc/tp_qt_widgets/Globals.h

