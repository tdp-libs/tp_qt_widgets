#ifndef tp_qt_widgets_DebugWidget_h
#define tp_qt_widgets_DebugWidget_h

#include "tp_qt_widgets/Globals.h"

#include <QPlainTextEdit>

namespace tp_qt_widgets
{

//##################################################################################################
//! Displays tpDebug and tpWarning in a widget.
class TP_QT_WIDGETS_SHARED_EXPORT DebugWidget: public QPlainTextEdit
{
  Q_OBJECT
  TP_DQ;
public:
  //################################################################################################
  DebugWidget(QWidget* parent=nullptr);

  //################################################################################################
  ~DebugWidget() override;
};

}

#endif
