#include "tp_qt_widgets/BusyMessage.h"
#include "tp_qt_widgets/ProgressBar.h"
#include "tp_qt_widgets/AnimationWidget.h"

#include "tp_qt_utils/Progress.h"

#include "tp_utils/DebugUtils.h"

#include <QWidget>
#include <QThread>
#include <QPointer>
#include <QEvent>
#include <QEventLoop>
#include <QBoxLayout>
#include <QLabel>

#include <QDebug>

namespace tp_qt_widgets
{

//##################################################################################################
struct BusyMessage::Private: public QThread
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::BusyMessage::Private");
  TP_NONCOPYABLE(Private);

  friend class BusyMessage;
  BusyMessage* q;
  BusyMessageConfiguration configuration;

  tp_qt_utils::Progress progress;

  QEventLoop* eventLoop;

  QString message;
  int stage;
  QPointer<QLabel> label;



  //################################################################################################
  Private(BusyMessage* q_, BusyMessageConfiguration configuration_):
    q(q_),
    configuration(std::move(configuration_)),
    eventLoop(new QEventLoop()),
    stage(0),
    label(nullptr)
  {
    setObjectName("BusyMessage");

    if(q->parent())
      q->parent()->installEventFilter(this);

    startTimer(200);
  }

  //################################################################################################
  ~Private() final
  {
    progress.stop(true);
    wait(3000);

    if(isRunning())
      tpWarning() << "BusyMessage task failed to stop after stop request!";

    eventLoop->deleteLater();
  }

  //################################################################################################
  bool eventFilter(QObject* watched, QEvent* event) final
  {
    TP_UNUSED(watched);

    if(event->type() == QEvent::Resize)
    {
      resize();
    }

    return QThread::eventFilter(watched, event);
  }

  //################################################################################################
  void timerEvent(QTimerEvent* event) final
  {
    TP_UNUSED(event);

    if(label)
    {
      message = progress.description();

      QString temp = message;

      //Only animate the text
      if(!configuration.animation.isValid())
      {
        stage = (stage+1)%12;
        for(int c=0; c<(stage/3); c++)
          temp.append('.');
      }

      label->setText(temp);
    }

    if(!isRunning())
      eventLoop->exit();
  }

  //################################################################################################
  void resize()
  {
    auto parentWidget = qobject_cast<QWidget*>(q->parent());

    if(!parentWidget)
      return;

    q->setGeometry(parentWidget->geometry());
  }

  //################################################################################################
  void run() final
  {
    configuration.taskCallback(&progress, configuration.opaque);
  }
};

//##################################################################################################
BusyMessage::BusyMessage(const BusyMessageConfiguration& configuration, QWidget* parent):
  QFrame(parent),
  d(new Private(this, configuration))
{
  d->start();

  auto layout = new QVBoxLayout(this);
  layout->addStretch(5);

  if(configuration.animation.isValid())
  {
    auto animationWidget = new AnimationWidget();
    animationWidget->setAnimation(configuration.animation);
    animationWidget->setFPS(configuration.fps);
    animationWidget->setAttribute(Qt::WA_TranslucentBackground);
    layout->addWidget(animationWidget, 0, Qt::AlignCenter);
  }

  d->message = "Busy";
  d->label = new QLabel();
  d->label->setAlignment(Qt::AlignCenter);
  d->label->setWordWrap(true);
  d->label->setObjectName("BusyMessageLabel");
  layout->addWidget(d->label);

  if(configuration.showProgress)
    layout->addWidget(new ProgressBar(&d->progress));

  layout->addStretch(9);

  d->resize();
}

//##################################################################################################
BusyMessage::~BusyMessage()
{
  delete d;
}

//##################################################################################################
void BusyMessage::performTask(const BusyMessageConfiguration& configuration, QWidget* parent)
{
  QPointer<BusyMessage> busyMessage = new BusyMessage(configuration, parent);
  busyMessage->show();
  busyMessage->exec();
  delete busyMessage;
}

//##################################################################################################
void BusyMessage::exec()
{
  d->eventLoop->exec();
}

}
