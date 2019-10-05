#ifndef tp_qt_widgets_FontUtils_h
#define tp_qt_widgets_FontUtils_h

#include "tp_qt_widgets/Globals.h"

#include <QFont>

namespace tp_qt_widgets
{
//##################################################################################################
QFont tp_qt_WIDGETS_SHARED_EXPORT fontFromString(QString fontString);

//##################################################################################################
QString tp_qt_WIDGETS_SHARED_EXPORT fontToString(const QFont& font);

//##################################################################################################
//! Convers em to pixels
int tp_qt_WIDGETS_SHARED_EXPORT pixelSize(float em, const QFont& font=QFont());
}

#endif
