#ifndef tdp_qt_widgets_FontUtils_h
#define tdp_qt_widgets_FontUtils_h

#include "tdp_qt_widgets/Globals.h"

#include <QFont>

namespace tdp_qt_widgets
{

//##################################################################################################
Qt::BrushStyle TDP_QT_WIDGETS_SHARED_EXPORT brushStyleFromString(const QString& brushStyleString);


//##################################################################################################
Qt::PenStyle TDP_QT_WIDGETS_SHARED_EXPORT penStyleFromString(const QString& penStyleString);


//##################################################################################################
Qt::PenCapStyle TDP_QT_WIDGETS_SHARED_EXPORT penCapStyleFromString(const QString& penCapStyleString);


//##################################################################################################
Qt::PenJoinStyle TDP_QT_WIDGETS_SHARED_EXPORT penJointStyleFromString(const QString& penJointStyleString);


//##################################################################################################
Qt::Alignment TDP_QT_WIDGETS_SHARED_EXPORT alignmentFromString(const QString& penJointStyleString);

}

#endif
