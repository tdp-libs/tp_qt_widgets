#include "tp_qt_widgets/ScrollLayoutAnimation.h"
#include "tp_qt_widgets/ScrollLayout.h"

#include <QPropertyAnimation>
#include <QEasingCurve>

namespace tp_qt_widgets
{
//##################################################################################################
ScrollLayoutAnimation::ScrollLayoutAnimation(QObject* layout):
  m_animation(new QPropertyAnimation(layout, "animationSpacing"))
{

}

//##################################################################################################
void ScrollLayoutAnimation::animate(ScrollLayoutAnimationType type, int duration)
{
  TP_UNUSED(type);

  m_animation->setDuration(duration);
  m_animation->setEasingCurve(QEasingCurve::OutExpo);
  m_animation->setStartValue(1000);
  m_animation->setEndValue(0);
  m_animation->start();
}

}
