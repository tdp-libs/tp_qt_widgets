#include "tp_qt_widgets/AnimationWidget.h"
#include "tp_qt_widgets/Animation.h"

#include <QTimerEvent>
#include <QPainter>

#include <QDebug>

namespace tp_qt_widgets
{
//##################################################################################################
struct AnimationWidget::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::AnimationWidget::Private");
  TP_NONCOPYABLE(Private);

  AnimationWidget* q;

  Animation animation;
  int currentFrame;
  int fps;
  int timerID;

  //################################################################################################
  Private(AnimationWidget* q_):
    q(q_),
    currentFrame(0),
    fps(12),
    timerID(0)
  {

  }

  //################################################################################################
  void updateCurrentFrame()
  {
    currentFrame++;

    if(currentFrame>=animation.frameCount())
      currentFrame = 0;

    q->update();
  }
};

//##################################################################################################
AnimationWidget::AnimationWidget(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  TP_QT_ADD_TOOL_TIP();
}

//##################################################################################################
AnimationWidget::~AnimationWidget()
{
  delete d;
}

//##################################################################################################
void AnimationWidget::setAnimation(const Animation& animation)
{
  d->animation = animation;

  if(d->timerID)
    killTimer(d->timerID);

  d->timerID = startTimer(1000 / d->fps);
  d->updateCurrentFrame();
}

//##################################################################################################
void AnimationWidget::setFPS(int fps)
{
  d->fps = fps;

  if(d->timerID)
    killTimer(d->timerID);

  d->timerID = startTimer(1000 / d->fps);
}

//################################################################################################
QSize	AnimationWidget::sizeHint()const
{
  return QSize(d->animation.width(), d->animation.height());
}

//##################################################################################################
void AnimationWidget::timerEvent(QTimerEvent* event)
{
  if(d->timerID == event->timerId())
    d->updateCurrentFrame();
  else
    QWidget::timerEvent(event);
}

//##################################################################################################
void AnimationWidget::paintEvent(QPaintEvent* event)
{
  TP_UNUSED(event);
  QPainter painter(this);

  d->animation.setScale(painter.deviceTransform().m11(), painter.deviceTransform().m22());

  int w = d->animation.width();
  int h = d->animation.height();
  int x = (width() - w)/2;
  int y = (height() - h)/2;

  painter.drawPixmap(x, y, w, h, d->animation.framePixmap(d->currentFrame));
}

}
