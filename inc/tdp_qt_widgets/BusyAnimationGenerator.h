#ifndef tdp_qt_widgets_BusyAnimationGenerator_h
#define tdp_qt_widgets_BusyAnimationGenerator_h

#include "tdp_qt_widgets/Globals.h"

#include <QImage>

namespace tdp_qt_widgets
{

//##################################################################################################
//! An animation made up from key frames
/*!

*/
class TDP_QT_WIDGETS_SHARED_EXPORT BusyAnimationGenerator
{
public:
  //################################################################################################
  BusyAnimationGenerator();

  //################################################################################################
  virtual ~BusyAnimationGenerator();

  //################################################################################################
  QImage generateImage(int size)const;

  //################################################################################################
  void setColor(const QColor& color);

  //################################################################################################
  //0 -> 1
  void setMaxAngle(float maxAngle);

  //################################################################################################
  //0 -> 1
  void setLineWidth(float lineWidth);

  //################################################################################################
  void setClockwise(bool clockwise);

  //################################################################################################
  void setRepeat(int repeat);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
