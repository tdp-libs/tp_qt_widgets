#include "tp_qt_widgets/WheelSafeScrollArea.h"

#include <QWheelEvent>
#include <QCoreApplication>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QScrollBar>

namespace tp_qt_widgets
{

//##################################################################################################
struct WheelSafeScrollArea::Private
{
  QObjectList watchedObjects;
};

//##################################################################################################
WheelSafeScrollArea::WheelSafeScrollArea():
  d(new Private())
{

}

//##################################################################################################
WheelSafeScrollArea::~WheelSafeScrollArea()
{
  for(auto object : d->watchedObjects)
    disconnect(object, &QObject::destroyed, this, &WheelSafeScrollArea::childDestroyed);
  delete d;
}

//##################################################################################################
bool WheelSafeScrollArea::eventFilter(QObject* watched, QEvent* event)
{
  if(event->type() == QEvent::Wheel && d->watchedObjects.contains(watched))
  {
    QWheelEvent* wheelEvent = reinterpret_cast<QWheelEvent*>(event);

    if(qAbs(wheelEvent->angleDelta().x()) > qAbs(wheelEvent->angleDelta().y()))
      QCoreApplication::sendEvent(horizontalScrollBar(), wheelEvent);
    else
      QCoreApplication::sendEvent(verticalScrollBar(), wheelEvent);

    return true;
  }

  return QScrollArea::eventFilter(watched, event);
}

//##################################################################################################
void WheelSafeScrollArea::updateWatchedObjects()
{
  QObjectList objects = children();

  while(!objects.isEmpty())
  {
    QObject* object = objects.takeLast();
    objects.append(object->children());

    if(d->watchedObjects.contains(object))
      continue;

    if(qobject_cast<QSlider*>(object)        ||
       qobject_cast<QSpinBox*>(object)       ||
       qobject_cast<QDoubleSpinBox*>(object) ||
       qobject_cast<QCheckBox*>(object)      ||
       qobject_cast<QComboBox*>(object))
    {
      connect(object, &QObject::destroyed, this, &WheelSafeScrollArea::childDestroyed);
      object->installEventFilter(this);
      d->watchedObjects.push_back(object);
    }
  }
}

//##################################################################################################
void WheelSafeScrollArea::childDestroyed(QObject* object)
{
  d->watchedObjects.removeOne(object);
}

//##################################################################################################
void WheelSafeScrollArea::showEvent(QShowEvent* event)
{
  QScrollArea::showEvent(event);
  updateWatchedObjects();
}

}
