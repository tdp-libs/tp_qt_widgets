#ifndef tp_qt_widgets_FontUtils_h
#define tp_qt_widgets_FontUtils_h

#include "tp_qt_widgets/Globals.h"

#include <QFont>

namespace tp_qt_widgets
{

//##################################################################################################
Qt::BrushStyle TP_QT_WIDGETS_SHARED_EXPORT brushStyleFromString(const QString& brushStyleString);


//##################################################################################################
Qt::PenStyle TP_QT_WIDGETS_SHARED_EXPORT penStyleFromString(const QString& penStyleString);


//##################################################################################################
Qt::PenCapStyle TP_QT_WIDGETS_SHARED_EXPORT penCapStyleFromString(const QString& penCapStyleString);


//##################################################################################################
Qt::PenJoinStyle TP_QT_WIDGETS_SHARED_EXPORT penJointStyleFromString(const QString& penJointStyleString);


//##################################################################################################
Qt::Alignment TP_QT_WIDGETS_SHARED_EXPORT alignmentFromString(const QString& penJointStyleString);

}

#endif
