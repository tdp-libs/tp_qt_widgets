#ifndef tp_qt_widgets_Animation_h
#define tp_qt_widgets_Animation_h

#include "tp_qt_widgets/Globals.h"

#include <QImage>
#include <QPixmap>
#include <QPoint>

namespace tp_qt_widgets
{

//##################################################################################################
enum AnimationKeyFrameMode
{
  FrameModeStep,
  FrameModeRotate
};

//##################################################################################################
struct AnimationKeyFrame
{
  QImage image;

  //! The center point that the frame is rotated arround.
  QPoint frameCenter;

  float startDegrees;
  float endDegrees;
  bool counterClockWise;

  int durationFrames;

  AnimationKeyFrameMode mode;

  AnimationKeyFrame():
    startDegrees(0.0),
    endDegrees(0.0),
    counterClockWise(false),
    durationFrames(1)
  {

  }
};

//##################################################################################################
//! An animation made up from key frames
/*!

*/
class TP_QT_WIDGETS_SHARED_EXPORT Animation
{
public:
  //################################################################################################
  //! Construct an empty animation
  /*!
  */
  Animation(int width=0, int height=0);

  Animation(const Animation& other);

  Animation& operator=(const Animation& other);

  //################################################################################################
  virtual ~Animation();

  //################################################################################################
  //! Returns the width of the animation
  /*!
  This returns the original size of the animation it does nnot take scale into account.
  */
  int width() const;

  //################################################################################################
  //! Returns the height of the animation
  /*!
  This returns the original size of the animation it does nnot take scale into account.
  */
  int height() const;

  //################################################################################################
  //! Add a frame to the animation
  void addKeyFrame(const AnimationKeyFrame& frame);

  //################################################################################################
  QImage frameImage(int frame) const;

  //################################################################################################
  QPixmap framePixmap(int frame) const;

  //################################################################################################
  int frameCount() const;

  //################################################################################################
  bool isValid() const;

  //################################################################################################
  //! This can be used to compensate for device scale
  /*!
  All generated images will be scaled by this value.

  \param scaleX - The amount to scale the x axis by.
  \param scaleY - The amount to sacle the y axis by.
  */
  void setScale(float scaleX, float scaleY);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
