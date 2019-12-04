#include "tp_qt_widgets/Globals.h"

#include <QToolTip>
#include <QHelpEvent>

namespace tp_qt_widgets
{

#ifdef TP_ADD_WIDGET_NAME_TOOL_TIPS
ClassNameToolTips::ClassNameToolTips(QObject* parent):
  QObject(parent)
{
}

bool ClassNameToolTips::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QString toolTip;

    while(obj)
    {
      toolTip += QString(obj->metaObject()->className()) + '\n';
      obj = obj->parent();
    }

    QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), toolTip);
    return true;
  }
  return QObject::eventFilter(obj, event);
}

#endif

}
