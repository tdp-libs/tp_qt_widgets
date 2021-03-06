#ifndef tp_qt_widgets_Utils_h
#define tp_qt_widgets_Utils_h

#include "tp_qt_widgets/Globals.h"

#include <QRectF>

namespace tp_qt_widgets
{

//##################################################################################################
//! Set the visibility of the on screen keyboard.
void TP_QT_WIDGETS_SHARED_EXPORT setKeyboardVisible(bool visible);

//##################################################################################################
QRectF TP_QT_WIDGETS_SHARED_EXPORT centerFit(float maxWidth, float maxHeight, const QRectF& source);

}

#endif
