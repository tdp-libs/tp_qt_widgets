#include "tp_qt_widgets/detail/ColorPicker_RGBSlider.h"

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
  int combindedWidth;
  int paddingLeft;

  //################################################################################################
  Geometry_lt(ColorPicker_RGBSlider* q):
    width(q->width()),
    height(q->height()),
    sliderWidth(20),
    sliderPadding(5),
    margin(9),
    previewWidth(height-(margin*2)),
    combindedWidth((sliderWidth*3) + previewWidth),
    paddingLeft((width - combindedWidth) / 2)
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
    int left = paddingLeft + (i*sliderWidth) + sliderPadding;
    return QRect(left, margin, sliderWidth-(2*sliderPadding), height-(margin*2));
  }

  //################################################################################################
  QPointF sliderHandleCenter(char component, double f)
  {
    f = 1.0 - f;
    auto r = sliderRect(component);
    double x = double(r.left()) + (double(r.width()) / 2.0);
    double y = double(r.top()) + (double(r.height()) * f);
    return QPointF(x, y);
  }

  //################################################################################################
  QRect sliderHandleRect(char component, double f)
  {
    int i=sliderIndex(component);
    f = 1.0 - f;

    int left = paddingLeft + (i*sliderWidth);
    double y = double(margin) + (double(height-(margin*2)) * f);
    double sliderRadius = double(sliderWidth) / 2.0;
    return QRect(left, int(y-sliderRadius), sliderWidth, sliderWidth);
  }

  //################################################################################################
  QRect prevewRect()
  {
    int left = paddingLeft + (sliderWidth*3) + sliderPadding ;
    return QRect(left, margin, previewWidth, height-(margin*2));
  }
};
}

//##################################################################################################
struct ColorPicker_RGBSlider::Private
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
    float v = 1.0 - std::clamp(double(pos.y() - r.y()) / double(r.height()), 0.0, 1.0);

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
ColorPicker_RGBSlider::ColorPicker_RGBSlider(QWidget* parent):
  ColorPicker(parent),
  d(new Private(this))
{

}

//##################################################################################################
ColorPicker_RGBSlider::~ColorPicker_RGBSlider()
{
  delete d;
}

//##################################################################################################
void ColorPicker_RGBSlider::set(const QColor& color)
{
  d->color = color;
  update();
}

//##################################################################################################
QColor ColorPicker_RGBSlider::get() const
{
  return d->color;
}

//##################################################################################################
void ColorPicker_RGBSlider::paintEvent(QPaintEvent *event)
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

    QLinearGradient gradient(0,r.bottom(),0,r.top());
    gradient.setColorAt(1.0, color(255));
    gradient.setColorAt(f  , d->color  );
    gradient.setColorAt(0.0, color(0  ));

    {
      double radius = double(r.width())/2.0;
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
void ColorPicker_RGBSlider::mouseMoveEvent(QMouseEvent* event)
{
  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_RGBSlider::mousePressEvent(QMouseEvent* event)
{
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
  else                 d->interaction = 0;

  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_RGBSlider::mouseReleaseEvent(QMouseEvent* event)
{
  if(d->interaction)
    event->accept();

  d->interaction = 0;
}

}

}
