#ifndef tdp_qt_widgets_FontUtils_h
#define tdp_qt_widgets_FontUtils_h

#include "tdp_qt_widgets/Globals.h"

#include <QFont>

namespace tdp_qt_widgets
{
//##################################################################################################
QFont TDP_QT_WIDGETS_SHARED_EXPORT fontFromString(QString fontString);

//##################################################################################################
QString TDP_QT_WIDGETS_SHARED_EXPORT fontToString(const QFont& font);

//##################################################################################################
//! Convers em to pixels
int TDP_QT_WIDGETS_SHARED_EXPORT pixelSize(float em, const QFont& font=QFont());
}

#endif
