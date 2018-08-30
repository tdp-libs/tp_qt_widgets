#include "tdp_qt_widgets/Scroller.h"

#include <QAbstractScrollArea>
#include <QEvent>
#include <QMouseEvent>
#include <QChildEvent>
#include <QApplication>
#include <QScrollBar>
#include <QPointer>
#include <QDateTime>
#include <QDebug>

#include <cmath>

#define REFRESH_RATE 15

namespace tdp_qt_widgets
{
namespace
{
struct EventDetails_lt
{
  QObject* object;
  QEvent* event;
  QEvent::Type type;

  EventDetails_lt(QObject* object_, QEvent* event_):
    object(object_),
    event(event_),
    type(event->type())
  {

  }
};
}

//##################################################################################################
struct Scroller::Private
{
  Scroller* q;
  QAbstractScrollArea* scrollArea;

  QDateTime lastRecording;
  QPoint pressPosition;
  QPoint lastMovement;

  QSet<QObject*> filteredObjects;

  QList<EventDetails_lt*> eventsToIgnore;

  int timerID{0};
  float speed{0.0f};
  float mostRecentSpeed{0.0f};

  bool isPressed{false};
  bool movementStarted{false};

  //################################################################################################
  Private(Scroller* q_, QAbstractScrollArea* scrollArea_):
    q(q_),
    scrollArea(scrollArea_)
  {

  }

  //################################################################################################
  void addEvent(QObject* object, QEvent* event)
  {
    eventsToIgnore.append(new EventDetails_lt(object, event));

    if(eventsToIgnore.size()>10)
      delete eventsToIgnore.takeFirst();
  }

  //################################################################################################
  bool ignoreEvent(QObject* object, QEvent* event)
  {
    for(int i=eventsToIgnore.size()-1; i>=0; i--)
    {
      EventDetails_lt* eventDetails = eventsToIgnore.at(i);
      if(eventDetails->object == object && eventDetails->event==event && eventDetails->type == event->type())
        return true;
    }

    return false;
  }

  //################################################################################################
  void setSpeed(float speed_)
  {
    mostRecentSpeed = 0.0f;
    speed = speed_;
    if(!timerID)
      timerID = q->startTimer(REFRESH_RATE);
  }
};

//##################################################################################################
Scroller::Scroller(QAbstractScrollArea* scrollArea):
  QObject(scrollArea),
  d(new Private(this, scrollArea))
{
  scrollArea->installEventFilter(this);
  scrollArea->viewport()->installEventFilter(this);
}

//##################################################################################################
Scroller::~Scroller()
{
  qDeleteAll(d->eventsToIgnore);
  delete d;
}

//##################################################################################################
bool Scroller::eventFilter(QObject* object, QEvent* event)
{
  if(d->ignoreEvent(object, event))
    return false;

  QWidget* widget = nullptr;
  if(object->isWidgetType())
    widget = dynamic_cast<QWidget*>(object);

  //Get the position of mouse events
  QPoint pos;
  switch(event->type())
  {
  case QEvent::MouseButtonPress: case QEvent::MouseButtonRelease: case QEvent::MouseMove:
  {
    if(widget)
    {
      auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
      if(mouseEvent)
        pos = widget->mapTo(d->scrollArea, mouseEvent->pos());
    }
    break;
  }

  default:
    break;
  };

  switch(event->type())
  {
  //-- Child events --------------------------------------------------------------------------------
  //Each time a child is added we need to install an event filter for it, this is because the mouse
  //event will go to the lowest child first and we need to catch them.
  case QEvent::ChildAdded:
  {
    auto childEvent = dynamic_cast<QChildEvent*>(event);
    if(childEvent && childEvent->child())
    {
      QObjectList children;
      children.append(childEvent->child());
      while(!children.isEmpty())
      {
        QObject* child = children.takeLast();
        if(!d->filteredObjects.contains(child))
        {
          child->installEventFilter(this);
          d->filteredObjects.insert(child);
          children.append(child->children());
        }
      }
    }
    break;
  }

  case QEvent::MouseButtonPress:
  {
    d->isPressed = true;
    d->movementStarted = false;
    d->pressPosition = pos;
    d->lastMovement = pos;
    d->setSpeed(0.0f);
    d->lastRecording = QDateTime::currentDateTime();
    d->mostRecentSpeed = 0.0f;
    return true;
  }

  case QEvent::MouseButtonRelease:
  {
    if(!widget)
      break;

    d->isPressed = false;
    if(!d->movementStarted)
    {
      QPoint mappedPress = widget->mapFrom(d->scrollArea, d->pressPosition);
      QPoint mappedRelease = widget->mapFrom(d->scrollArea, pos);

      QEvent* pressEvent   = new QMouseEvent(QEvent::MouseButtonPress, mappedPress, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
      QEvent* releaseEvent = new QMouseEvent(QEvent::MouseButtonRelease, mappedRelease, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

      //This is a bit of a hack but it should be safe, it allows us to avoid intercepting events
      //that we have created.
      d->addEvent(object, pressEvent);
      d->addEvent(object, releaseEvent);

      QApplication::postEvent(object, pressEvent);
      QApplication::postEvent(object, releaseEvent);
    }
    else
      d->setSpeed(d->mostRecentSpeed);

    return true;
  }

  case QEvent::MouseMove:
  {
    if(!d->isPressed)
      return false;

    if(!d->movementStarted)
    {
      if(QPoint(pos - d->pressPosition).manhattanLength() > 20)
        d->movementStarted = true;
      else
        return true;
    }

    int dy = pos.y()-d->lastMovement.y();

    float msecs = d->lastRecording.msecsTo(QDateTime::currentDateTime());
    float frames = msecs / float(REFRESH_RATE);
    if(frames>0.001f)
    {
      float distancePerFrame = tpBound(-100.0f, dy/frames, 100.0f);

      //Encourage acceleration, this helps to make short swift flicks work
      //and prevents it from slowing if the swipe slows a little towards the
      //end of the stroke.
      if(std::fabs(distancePerFrame) > std::fabs(d->mostRecentSpeed))
        d->mostRecentSpeed = distancePerFrame;
      else
        d->mostRecentSpeed = d->mostRecentSpeed*0.60f + distancePerFrame*0.40f;
    }

    d->scrollArea->verticalScrollBar()->setValue(d->scrollArea->verticalScrollBar()->value()-dy);
    d->lastMovement = pos;
    d->lastRecording = QDateTime::currentDateTime();
    return true;
  }

  default:
    break;
  }

  return false;
}

//##################################################################################################
void Scroller::timerEvent(QTimerEvent* event)
{
  if(event->timerId() == d->timerID)
  {
    d->speed*=0.95f;
    QScrollBar* sb = d->scrollArea->verticalScrollBar();

    if(std::fabs(d->speed) < 0.1f || sb->value() == ((d->speed>0.0f)?sb->maximum():sb->minimum()))
    {
      killTimer(d->timerID);
      d->timerID = 0;
      return;
    }

    sb->setValue(sb->value()-int(d->speed));
  }
  else
    QObject::timerEvent(event);
}

}
