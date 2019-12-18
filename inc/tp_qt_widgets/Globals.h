#ifndef tp_qt_widgets_Globals_h
#define tp_qt_widgets_Globals_h

#include "tp_qt_utils/Globals.h"

#include <QtCore/qglobal.h>

#include <functional>

#if defined(TP_QT_WIDGETS_LIBRARY)
#  define TP_QT_WIDGETS_SHARED_EXPORT TP_EXPORT
#else
#  define TP_QT_WIDGETS_SHARED_EXPORT TP_IMPORT
#endif

class QListView;
class QTextEdit;

//##################################################################################################
//! Widgets for displaying data types defined in tp_qt_utils
/*!
This module provides a set of widgets for displaying the data types defined in tp_qt_utils.
*/
namespace tp_qt_widgets
{

//##################################################################################################
void stayAtBottom(QListView* listView);

//##################################################################################################
std::function<void()> keepTextSelection(QTextEdit* textEdit);

#ifdef TP_ADD_WIDGET_NAME_TOOL_TIPS
class ClassNameToolTips : public QObject
{
public:
  ClassNameToolTips(QObject* parent);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif

}

#endif
