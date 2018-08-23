#ifndef tdp_qt_widgets_AnimationWidget_h
#define tdp_qt_widgets_AnimationWidget_h

#include "tdp_qt_widgets/Globals.h"

#include <QWidget>

namespace tdp_qt_widgets
{
class Animation;

//##################################################################################################
//! An animation made up from key frames
/*!

*/
class TDP_QT_WIDGETS_SHARED_EXPORT AnimationWidget: public QWidget
{
  Q_OBJECT
public:
  //################################################################################################
  //! Construct an empty animation
  /*!
  */
  AnimationWidget(QWidget* parent = nullptr);

  //################################################################################################
  virtual ~AnimationWidget();

  //################################################################################################
  //! Set the animation
  void setAnimation(const Animation& animation);

  //################################################################################################
  void setFPS(int fps);

  //################################################################################################
  virtual QSize	sizeHint()const;

protected:
  virtual void timerEvent(QTimerEvent* event);

  virtual void paintEvent(QPaintEvent* event);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
