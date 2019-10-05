#ifndef tp_qt_widgets_ScrollLayoutAnimation_h
#define tp_qt_widgets_ScrollLayoutAnimation_h

#include "tp_qt_widgets/Globals.h"

#include <QObject>

class TP_QT_WIDGETS_SHARED_EXPORT QPropertyAnimation;

namespace tp_qt_widgets
{

//##################################################################################################
enum TP_QT_WIDGETS_SHARED_EXPORT ScrollLayoutAnimationType
{
  CollapseAnimation
};

//##################################################################################################
class TP_QT_WIDGETS_SHARED_EXPORT ScrollLayoutAnimation: public QObject
{
  QPropertyAnimation *m_animation;
  Q_OBJECT
public:
  //################################################################################################
  ScrollLayoutAnimation(QObject* layout);

  //################################################################################################
  void animate(ScrollLayoutAnimationType type, int duration);

};
}

#endif
