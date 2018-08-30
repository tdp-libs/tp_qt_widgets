#include "tdp_qt_widgets/Animation.h"

#include <QList>
#include <QPainter>
#include <QTransform>

#include <cmath>

namespace tdp_qt_widgets
{
namespace
{
struct FrameDetails_lt
{
  QImage image;
  QPixmap pixmap;

  FrameDetails_lt(QImage image_):
    image(std::move(image_)),
    pixmap(QPixmap::fromImage(image))
  {

  }
};
}

//##################################################################################################
struct Animation::Private
{
  QList<AnimationKeyFrame> keyFrames;

  QList<FrameDetails_lt> frames;

  int width;
  int height;

  float scaleX{1.0f};
  float scaleY{1.0f};

  bool refreshRequired{true};

  //################################################################################################
  Private(int width_, int height_):
    width(width_),
    height(height_)
  {

  }

  //################################################################################################
  Private& operator=(const Private& other)=default;

  //################################################################################################
  Private(const Private& other)=default;

  //################################################################################################
  Private& operator=(Private&& other)=delete;

  //################################################################################################
  Private(Private&& other)=delete;

  //################################################################################################
  ~Private()=default;

  //################################################################################################
  void generateFrames()
  {
    if(!refreshRequired)
      return;

    refreshRequired = false;

    frames.clear();

    auto adjustedWidth  = int(float(width)*scaleX);
    auto adjustedHeight = int(float(height)*scaleY);

    Q_FOREACH(const AnimationKeyFrame& keyFrame, keyFrames)
    {
      QImage resized = keyFrame.image.scaled(adjustedWidth, adjustedHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

      switch(keyFrame.mode)
      {
      case FrameModeStep:
        for(int i=0; i<keyFrame.durationFrames; i++)
          frames.append(resized);
        break;

      case FrameModeRotate:
        float degrees = keyFrame.endDegrees - keyFrame.startDegrees;
        if(degrees<0.0f)
          degrees = 360.0f + degrees;

        qreal degreesPerFrame = qreal(degrees) / qreal(keyFrame.durationFrames);

        for(int i=0; i<keyFrame.durationFrames; i++)
        {
          qreal angle = degreesPerFrame * qreal(i) + qreal(keyFrame.startDegrees);
          if(angle>qreal(360.0))
            angle = angle - qreal(360.0);

          //Draw a rotated scaled image
          QImage image(adjustedWidth, adjustedHeight, QImage::Format_ARGB32);
          image.fill(0);
          QPainter painter(&image);
          painter.setRenderHint(QPainter::SmoothPixmapTransform);

          QTransform transform;
          transform.translate(qreal(adjustedWidth)/qreal(2.0), qreal(adjustedHeight)/qreal(2.0));
          transform.rotate(angle);
          transform.scale(qreal(adjustedWidth)/qreal(keyFrame.image.width()), qreal(adjustedHeight)/qreal(keyFrame.image.height()));
          transform.translate(-qreal(keyFrame.image.width())/qreal(2.0), -qreal(keyFrame.image.height())/qreal(2.0));
          painter.setTransform(transform);

          painter.drawImage(0, 0, keyFrame.image);

          frames.append(image);
        }

        break;
      }
    }
  }
};

//##################################################################################################
Animation::Animation(int width, int height):
  d(new Private(width, height))
{

}

//##################################################################################################
Animation::Animation(const Animation& other):
  d(new Private(*other.d))
{

}

//##################################################################################################
Animation& Animation::operator=(const Animation& other)
{
  *d = *other.d;  
  return *this;
}

//##################################################################################################
Animation::~Animation()
{
  delete d;
}

//##################################################################################################
int Animation::width()const
{
  return d->width;
}

//##################################################################################################
int Animation::height()const
{
  return d->height;
}

//##################################################################################################
void Animation::addKeyFrame(const AnimationKeyFrame& frame)
{
  d->refreshRequired = true;
  d->keyFrames.append(frame);
}

//##################################################################################################
QImage Animation::frameImage(int frame)const
{
  d->generateFrames();

  if(frame<0 || frame>=d->frames.size())
    return QImage();

  return d->frames.at(frame).image;
}

//##################################################################################################
QPixmap Animation::framePixmap(int frame)const
{
  d->generateFrames();

  if(frame<0 || frame>=d->frames.size())
    return QPixmap();

  return d->frames.at(frame).pixmap;
}

//##################################################################################################
int Animation::frameCount()const
{
  d->generateFrames();
  return d->frames.size();
}

//##################################################################################################
bool Animation::isValid()const
{
  return !d->keyFrames.isEmpty();
}

//##################################################################################################
void Animation::setScale(float scaleX, float scaleY)
{
  if(std::fabs(scaleX-d->scaleX)>0.001f || std::fabs(scaleY-d->scaleY)>0.001f)
  {
    d->scaleX = scaleX;
    d->scaleY = scaleY;
    d->refreshRequired = true;
  }
}

}
