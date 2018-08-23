#ifndef tdp_qt_widgets_Utils_h
#define tdp_qt_widgets_Utils_h

#include "tdp_qt_widgets/Globals.h"

#include <QRectF>

namespace tdp_qt_widgets
{

//##################################################################################################
//! Set the visibility of the on screen keyboard.
void TDP_QT_WIDGETS_SHARED_EXPORT setKeyboardVisible(bool visible);

//##################################################################################################
QRectF TDP_QT_WIDGETS_SHARED_EXPORT centerFit(float maxWidth, float maxHeight, const QRectF& source);

}

#endif
