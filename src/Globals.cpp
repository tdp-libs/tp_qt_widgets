#include "tp_qt_widgets/Globals.h"

#include <QToolTip>
#include <QHelpEvent>
#include <QListView>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QtPlugin>
#include <QTextEdit>

#include <memory>

void tp_qt_widgets_staticInitImpl()
{
#ifdef TP_LINUX
#ifdef TP_QT_STATIC
  Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
  Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)
#endif
#endif
}

namespace tp_qt_widgets
{

//##################################################################################################
int staticInit()
{
  tp_qt_widgets_staticInitImpl();
  return 0;
}

//##################################################################################################
void stayAtBottom(QAbstractItemView* listView)
{
  std::shared_ptr<bool> atBottom{new bool};
  QObject::connect(listView->model(), &QAbstractItemModel::rowsAboutToBeInserted, listView, [=]
  {
    auto sb = listView->verticalScrollBar();
    *atBottom = sb?(sb->value()==sb->maximum()):true;
  });
  QObject::connect(listView->model(), &QAbstractItemModel::rowsInserted, listView, [=]
  {
    if(*atBottom)
      listView->scrollToBottom();
  });
}


//##################################################################################################
std::function<void()> keepTextSelection(QTextEdit* textEdit)
{
  auto cursor = textEdit->textCursor();
  auto start = cursor.selectionStart();
  auto end = cursor.selectionEnd();
  return [=]
  {
    auto c = textEdit->textCursor();
    c.setPosition(start);
    c.setPosition(end, QTextCursor::KeepAnchor);
    textEdit->setTextCursor(c);
  };
}

#ifdef TP_ADD_WIDGET_NAME_TOOL_TIPS
//##################################################################################################
ClassNameToolTips::ClassNameToolTips(QObject* parent):
  QObject(parent)
{
}

//##################################################################################################
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
