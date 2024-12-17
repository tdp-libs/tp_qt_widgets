TARGET = tp_qt_widgets
TEMPLATE = lib

DEFINES += TP_QT_WIDGETS_LIBRARY

SOURCES += src/Utils.cpp
HEADERS += inc/tp_qt_widgets/Utils.h

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

SOURCES += src/detail/ColorPicker.cpp
HEADERS += inc/tp_qt_widgets/detail/ColorPicker.h

SOURCES += src/detail/ColorPicker_HSVCircle.cpp
HEADERS += inc/tp_qt_widgets/detail/ColorPicker_HSVCircle.h

SOURCES += src/detail/ColorPicker_HSVSquare.cpp
HEADERS += inc/tp_qt_widgets/detail/ColorPicker_HSVSquare.h

SOURCES += src/detail/ColorPicker_RGBSlider.cpp
HEADERS += inc/tp_qt_widgets/detail/ColorPicker_RGBSlider.h

SOURCES += src/detail/ColorPicker_RGBSlider_Horizontal.cpp
HEADERS += inc/tp_qt_widgets/detail/ColorPicker_RGBSlider_Horizontal.h

SOURCES += src/ColorPickerDialog.cpp
HEADERS += inc/tp_qt_widgets/ColorPickerDialog.h

SOURCES += src/ColorButton.cpp
HEADERS += inc/tp_qt_widgets/ColorButton.h

SOURCES += src/DebugWidget.cpp
HEADERS += inc/tp_qt_widgets/DebugWidget.h

SOURCES += src/CollapsiblePanel.cpp
HEADERS += inc/tp_qt_widgets/CollapsiblePanel.h

SOURCES += src/ExclusiveCollapsibleStack.cpp
HEADERS += inc/tp_qt_widgets/ExclusiveCollapsibleStack.h

SOURCES += src/SpinSlider.cpp
HEADERS += inc/tp_qt_widgets/SpinSlider.h

SOURCES += src/Globals.cpp
HEADERS += inc/tp_qt_widgets/Globals.h

