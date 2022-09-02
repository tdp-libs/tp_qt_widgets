#include "tp_qt_widgets/detail/ColorPicker_HSVSquare.h"

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
  Geometry_lt(ColorPicker_HSVSquare* q):
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
    case 'h': return 0;
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
    int left = paddingLeft + sliderWidth + sliderPadding ;
    return QRect(left, margin, previewWidth, height-(margin*2));
  }
};
}

//##################################################################################################
struct ColorPicker_HSVSquare::Private
{
  ColorPicker_HSVSquare* q;
  QColor color{Qt::gray};
  char interaction{0};

  //################################################################################################
  Private(ColorPicker_HSVSquare* q_):
    q(q_)
  {

  }

  //################################################################################################
  void update(const QPoint& pos)
  {
    QColor c = color;
    Geometry_lt geometry(q);

    if(interaction == 'h')
    {
      QRect r = geometry.sliderRect(interaction);

      int h;
      int s;
      int v;
      int a;
      color.getHsv(&h, &s, &v, &a);
      h = 1.0 - std::clamp(double(pos.y() - r.y()) / double(r.height()), 0.0, 1.0);
      color.setHsv(h, s, v);
    }

    if(c!=color)
    {
      emit q->colorChanged();
      q->update();
    }
  }
};

//##################################################################################################
ColorPicker_HSVSquare::ColorPicker_HSVSquare(QWidget* parent):
  ColorPicker(parent),
  d(new Private(this))
{

}

//##################################################################################################
ColorPicker_HSVSquare::~ColorPicker_HSVSquare()
{
  delete d;
}

//##################################################################################################
void ColorPicker_HSVSquare::set(const QColor& color)
{
  d->color = color;
  update();
}

//##################################################################################################
QColor ColorPicker_HSVSquare::get() const
{
  return d->color;
}

//##################################################################################################
void ColorPicker_HSVSquare::paintEvent(QPaintEvent *event)
{
  ColorPicker::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  Geometry_lt geometry(this);

  painter.setPen(Qt::NoPen);

  auto outline = Qt::white;

  auto drawSlider = [&](char component)
  {
    double f = getComponentF(component);
    QRect r = geometry.sliderRect(component);

    QLinearGradient gradient(0,r.height(),0,0);
    gradient.setColorAt(1.0, Qt::red);
    gradient.setColorAt(f  , Qt::green);
    gradient.setColorAt(0.0, Qt::blue);

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

  drawSlider('h');

  painter.setBrush(d->color);
  painter.drawRoundedRect(geometry.prevewRect(), 5.0, 5.0);
}

//##################################################################################################
void ColorPicker_HSVSquare::mouseMoveEvent(QMouseEvent* event)
{
  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_HSVSquare::mousePressEvent(QMouseEvent* event)
{
  Geometry_lt geometry(this);

  auto collision = [&](char component)
  {
    return
        geometry.sliderRect(component).contains(event->pos()) ||
        geometry.sliderHandleRect(component, getComponentF(component)).contains(event->pos());
  };

  if(collision('h'))d->interaction = 'h';
  else              d->interaction = 0;

  if(d->interaction)
    event->accept();

  d->update(event->pos());
}

//##################################################################################################
void ColorPicker_HSVSquare::mouseReleaseEvent(QMouseEvent* event)
{
  if(d->interaction)
    event->accept();

  d->interaction = 0;
}

}

}
