#ifndef tdp_qt_widgets_Globals_h
#define tdp_qt_widgets_Globals_h

#include "tp_utils/Globals.h"

#include <QtCore/qglobal.h>

#if defined(TDP_QT_WIDGETS_LIBRARY)
#  define TDP_QT_WIDGETS_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define TDP_QT_WIDGETS_SHARED_EXPORT Q_DECL_IMPORT
#endif

//##################################################################################################
//! Widgets for displaying data types defined in tdp_qt_utils
/*!
This module provides a set of widgets for displaying the data types defined in tdp_qt_utils.
*/
namespace tdp_qt_widgets{}

#endif
