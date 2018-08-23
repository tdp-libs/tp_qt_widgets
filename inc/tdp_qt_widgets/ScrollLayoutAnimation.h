#ifndef tdp_qt_widgets_ScrollLayoutAnimation_h
#define tdp_qt_widgets_ScrollLayoutAnimation_h

#include "tdp_qt_widgets/Globals.h"

#include <QObject>

class TDP_QT_WIDGETS_SHARED_EXPORT QPropertyAnimation;

namespace tdp_qt_widgets
{

//##################################################################################################
enum TDP_QT_WIDGETS_SHARED_EXPORT ScrollLayoutAnimationType
{
  CollapseAnimation
};

//##################################################################################################
class TDP_QT_WIDGETS_SHARED_EXPORT ScrollLayoutAnimation: public QObject
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
