#include "tp_qt_widgets/RefCountWidget.h"
#include "tp_utils/RefCount.h"

#include <QTableWidget>
#include <QTimerEvent>
#include <QMap>
#include <QBoxLayout>
#include <QTimerEvent>
#include <QPlainTextEdit>

namespace tp_qt_widgets
{

//##################################################################################################
struct RefCountWidget::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::RefCountWidget::Private");
  TP_NONCOPYABLE(Private);

  RefCountWidget* q;

  int timerID;

  QTableWidget* table;

  Private(RefCountWidget* q_):
    q(q_),
    timerID(0),
    table(nullptr)
  {

  }
};

//##################################################################################################
RefCountWidget::RefCountWidget(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  TP_QT_ADD_TOOL_TIP();

  new QVBoxLayout(this);
  layout()->setContentsMargins(0, 0, 0, 0);

#ifdef TP_REF_COUNT
  d->timerID = startTimer(1000);
  d->table = new QTableWidget;
  d->table->setColumnCount(3);
  d->table->setHorizontalHeaderLabels(QStringList{"Type", "#", "##"});
  layout()->addWidget(d->table);
#else
  auto message = new QPlainTextEdit();
  message->setReadOnly(true);
  message->setPlainText("Define TP_REF_COUNT in project.inc and rebuild for this display to work.");
  layout()->addWidget(message);
#endif
}

//##################################################################################################
RefCountWidget::~RefCountWidget()
{
  delete d;
}

//##################################################################################################
void RefCountWidget::timerEvent(QTimerEvent* event)
{
  if(event->timerId() != d->timerID)
    return QWidget::timerEvent(event);  

#ifdef TP_REF_COUNT
  std::unordered_map<std::string, tp_utils::InstanceDetails> instances;
  {
    tp_utils::RefCount::lock();
    for(const auto& i : tp_utils::RefCount::instances())
      instances[i.first.keyString()] = i.second;
    tp_utils::RefCount::unlock();
  }

  {
    d->table->setRowCount(int(instances.size()));
    int row=0;
    for(const auto& i : instances)
    {
      d->table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(i.first)));
      d->table->setItem(row, 1, new QTableWidgetItem(QString::number(i.second.count)));
      d->table->setItem(row, 2, new QTableWidgetItem(QString::number(i.second.total)));
      row++;
    }
  }

  d->table->resizeColumnsToContents();
#endif
}

}
