#ifndef tp_qt_widgets_FontUtils_h
#define tp_qt_widgets_FontUtils_h

#include "tp_qt_widgets/Globals.h"

#include <QFont>

namespace tp_qt_widgets
{
//##################################################################################################
QFont TP_QT_WIDGETS_SHARED_EXPORT fontFromString(QString fontString);

//##################################################################################################
QString TP_QT_WIDGETS_SHARED_EXPORT fontToString(const QFont& font);

//##################################################################################################
//! Convers em to pixels
int TP_QT_WIDGETS_SHARED_EXPORT pixelSize(float em, const QFont& font=QFont());
}

#endif
