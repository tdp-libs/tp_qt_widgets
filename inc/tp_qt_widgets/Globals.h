#ifndef tp_qt_widgets_Globals_h
#define tp_qt_widgets_Globals_h

#include "tp_qt_utils/Globals.h"

#include <QtCore/qglobal.h>

#if defined(tp_qt_WIDGETS_LIBRARY)
#  define tp_qt_WIDGETS_SHARED_EXPORT TP_EXPORT
#else
#  define tp_qt_WIDGETS_SHARED_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! Widgets for displaying data types defined in tp_qt_utils
/*!
This module provides a set of widgets for displaying the data types defined in tp_qt_utils.
*/
namespace tp_qt_widgets{}

#endif
