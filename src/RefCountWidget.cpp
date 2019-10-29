#include "tp_qt_widgets/RefCountWidget.h"
#include "tp_utils/RefCount.h"

#include <QTableWidget>
#include <QTimerEvent>
#include <QMap>
#include <QBoxLayout>
#include <QHeaderView>

namespace tp_qt_widgets
{

//##################################################################################################
struct RefCountWidget::Private
{
  RefCountWidget* q;

  int timerID;

  std::unordered_map<std::string, tp_utils::InstanceDetails> instances;

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

  d->timerID = startTimer(1000);

  new QVBoxLayout(this);
  layout()->setContentsMargins(0, 0, 0, 0);

  d->table = new QTableWidget;
  d->table->setColumnCount(3);
  d->table->setHorizontalHeaderLabels(QStringList{"Type", "#", "##"});
  layout()->addWidget(d->table);
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
  {
    tp_utils::RefCount::lock();
    for(const auto& i : tp_utils::RefCount::instances())
      d->instances[i.first.keyString()] = i.second;
    tp_utils::RefCount::unlock();
  }
#endif

  {
    d->table->setRowCount(int(d->instances.size()));
    int row=0;
    for(const auto& i : d->instances)
    {
      d->table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(i.first)));
      d->table->setItem(row, 1, new QTableWidgetItem(QString::number(i.second.count)));
      d->table->setItem(row, 2, new QTableWidgetItem(QString::number(i.second.total)));
      row++;
    }
  }

  d->table->resizeColumnsToContents();
}

}
