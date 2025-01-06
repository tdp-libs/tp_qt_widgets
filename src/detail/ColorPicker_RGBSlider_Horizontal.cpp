#include "tp_qt_widgets/detail/ColorPicker_RGBSlider_Horizontal.h"

#include <QPainter>
#include <QMouseEvent>

namespace tp_qt_widgets
{

namespace detail
{

namespace
{
//##################################################################################################
struct Geometry_lt
{
  int width;
  int height;
  int sliderWidth;
  int sliderPadding;
  int margin;
  int previewWidth;
  int paddingLeft;

  //################################################################################################
  Geometry_lt(const ColorPicker_RGBSlider_Horizontal* q):
    width(q->width()),
    height(q->height()),
    sliderWidth(20),
    sliderPadding(5),
    margin(9),
    previewWidth(height-(margin*2)),
    paddingLeft(margin)
  {

  }

  //################################################################################################
  int sliderIndex(char component)
  {
    switch(component)
    {
      case 'r': return 0;
      case 'g': return 1;
      case 'b': return 2;
    }
    return 0;
  }

  //################################################################################################
  QRect sliderRect(char component)
  {
    int i=sliderIndex(component);
    int left = paddingLeft + previewWidth + sliderPadding + margin;
    int top = margin + (i*sliderWidth) + sliderPadding;

    int cWidth = width - (left + margin);
    int cHeight = sliderWidth-(2*sliderPadding);

    return QRect(left, top, cWidth, cHeight);
  }

  //################################################################################################
  QPointF sliderHandleCenter(char component, double f)
  {
    auto r = sliderRect(component);
    double x = double(r.left()) + (double(r.width()) * f);
    double y = double(r.top()) + (double(r.height()) / 2.0);
    return QPointF(x, y);
  }

  //################################################################################################
  QRect sliderHandleRect(char component, double f)
  {
    auto r = sliderRect(component);
    int left = paddingLeft + previewWidth + sliderPadding + (double(r.width()) * f);
    double y = double(r.top()) + (double(r.height()) / 2.0);
    double sliderRadius = double(sliderWidth) / 2.0;
    return QRect(left, int(y-sliderRadius), sliderWidth, sliderWidth);
  }

  //################################################################################################
  QRect prevewRect()
  {
    int left = paddingLeft;
    return QRect(left, margin, previewWidth, height-(margin*2));
  }
};
}

//##################################################################################################
struct ColorPicker_RGBSlider_Horizontal::Private
{
  Q* q;
  QColor color;
  char interaction{0};

  //################################################################################################
  Private(Q* q_):
    q(q_)
  {

  }

  //################################################################################################
  void update(const QPoint& pos)
  {
    QColor c = color;
    Geometry_lt geometry(q);

    QRect r = geometry.sliderRect(interaction);
    float v = std::clamp(double(pos.x() - r.x()) / double(r.width()), 0.0, 1.0);

    if     (interaction == 'r') color.setRedF  (v);
    else if(interaction == 'g') color.setGreenF(v);
    else if(interaction == 'b') color.setBlueF (v);

    if(c!=color)
    {
      emit q->colorChanged();
      q->update();
    }
  }
};

//##################################################################################################
ColorPicker_RGBSlider_Horizontal::ColorPicker_RGBSlider_Horizontal(QWidget* parent):
  ColorPicker(parent),
  d(new Private(this))
{

}

//##################################################################################################
ColorPicker_RGBSlider_Horizontal::~ColorPicker_RGBSlider_Horizontal()
{
  delete d;
}

//##################################################################################################
void ColorPicker_RGBSlider_Horizontal::set(const QColor& color)
{
  d->color = color;
  update();
}

//##################################################################################################
QColor ColorPicker_RGBSlider_Horizontal::get() const
{
  return d->color;
}

//##################################################################################################
int ColorPicker_RGBSlider_Horizontal::targetHeight() const
{
  Geometry_lt geometry(this);
  return (2*geometry.margin) + (3*geometry.sliderWidth);
}

//##################################################################################################
void ColorPicker_RGBSlider_Horizontal::paintEvent(QPaintEvent *event)
{
  ColorPicker::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  Geometry_lt geometry(this);

  painter.setPen(Qt::NoPen);

  auto outline = Qt::white;

  auto drawSlider = [&](char component)
  {
    auto color = [&](uint8_t v)
    {
      QColor c = d->color;
      switch(component)
      {
        case 'r': c.setRed  (v); break;
        case 'g': c.setGreen(v); break;
        case 'b': c.setBlue (v); break;
      }
      return c;
    };

    double f = getComponentF(component);
    QRect r = geometry.sliderRect(component);

    QLinearGradient gradient(r.left(),0,r.right(),0);
    gradient.setColorAt(1.0, color(255));
    gradient.setColorAt(f  , d->color  );
    gradient.setColorAt(0.0, color(0  ));

    {
      double radius = double(r.height())/2.0;
      painter.save();
      painter.setBrush(gradient);
      painter.drawRoundedRect(r, radius, radius);
    }

    {
      double penWidth = 2;
      double radius = double(geometry.sliderWidth) / 2.0 - penWidth;
      painter.setBrush(d->color);
      painter.setPen(QPen(outline, penWidth));
      painter.drawEllipse(geometry.sliderHandleCenter(component, f), radius, radius);
    }

    painter.restore();
  };

  drawSlider('r');
  drawSlider('g');
  drawSlider('b');

  painter.setBrush(d->color);
  painter.drawRoundedRect(geometry.prevewRect(), 5.0, 5.0);
}

//##################################################################################################
void ColorPicker_RGBSlider_Horizontal::mouseMoveEvent(QMouseEvent* event)
{
  if(d->interaction == 0)
    return ColorPicker::mouseMoveEvent(event);

  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_RGBSlider_Horizontal::mousePressEvent(QMouseEvent* event)
{
  if(event->button() != Qt::LeftButton)
    return ColorPicker::mousePressEvent(event);

  Geometry_lt geometry(this);

  auto collision = [&](char component)
  {
    return
        geometry.sliderRect(component).contains(event->pos()) ||
        geometry.sliderHandleRect(component, getComponentF(component)).contains(event->pos());
  };

  if     (collision('r'))d->interaction = 'r';
  else if(collision('g'))d->interaction = 'g';
  else if(collision('b'))d->interaction = 'b';
  else                   d->interaction = 0;

  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_RGBSlider_Horizontal::mouseReleaseEvent(QMouseEvent* event)
{
  if(event->button() != Qt::LeftButton)
    return ColorPicker::mouseReleaseEvent(event);

  if(d->interaction)
    event->accept();

  d->interaction = 0;
}

}

}
