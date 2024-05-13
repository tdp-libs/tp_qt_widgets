#ifndef tp_qt_widgets_Globals_h
#define tp_qt_widgets_Globals_h

#include "tp_qt_utils/Globals.h" // IWYU pragma: keep

#include "tp_utils/TPPixel.h"

#include <QtCore/qglobal.h>
#include <QObject>  // IWYU pragma: keep

#include <functional>

#if defined(TP_QT_WIDGETS_LIBRARY)
#  define TP_QT_WIDGETS_SHARED_EXPORT TP_EXPORT
#else
#  define TP_QT_WIDGETS_SHARED_EXPORT TP_IMPORT
#endif

class QAbstractItemView;
class QTextEdit;
class QPlainTextEdit;

//##################################################################################################
//! Widgets for displaying data types defined in tp_qt_utils
/*!
This module provides a set of widgets for displaying the data types defined in tp_qt_utils.
*/
namespace tp_qt_widgets
{

//##################################################################################################
int staticInit();

//##################################################################################################
void stayAtBottom(QAbstractItemView* listView);

//##################################################################################################
std::function<void()> keepTextSelection(QTextEdit* textEdit);

//##################################################################################################
std::function<void()> keepTextSelection(QPlainTextEdit* textEdit);

#ifdef TP_ADD_WIDGET_NAME_TOOL_TIPS
//##################################################################################################
//! Add tool tips to all widgets containing the class name of the widget and its parents
/*!
Install like so:
\code{.cpp}
#ifdef TP_ADD_WIDGET_NAME_TOOL_TIPS
  app.installEventFilter(new tp_qt_widgets::ClassNameToolTips(&app));
#endif
\endcode

This relies on data added by moc so make sure your widget classes have Q_OBJECT macros in them.
*/
class ClassNameToolTips : public QObject
{
public:
  ClassNameToolTips(QObject* parent);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif

//##################################################################################################
TPPixel tpPixelFromQColor(const QColor& color);

//##################################################################################################
QColor tpPixelToQColor(const TPPixel& color);

}

#endif
