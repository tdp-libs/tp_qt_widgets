#include "tp_qt_widgets/detail/ColorPicker_HSVCircle.h"

#include <QPainter>
#include <QMouseEvent>

namespace tp_qt_widgets
{

namespace detail
{

//##################################################################################################
struct ColorPicker_HSVCircle::Private
{
  QColor color;
};

//##################################################################################################
ColorPicker_HSVCircle::ColorPicker_HSVCircle(QWidget* parent):
  ColorPicker(parent),
  d(new Private())
{

}

//##################################################################################################
ColorPicker_HSVCircle::~ColorPicker_HSVCircle()
{
  delete d;
}

//##################################################################################################
void ColorPicker_HSVCircle::set(const QColor& color)
{
  d->color = color;
  update();
}

//##################################################################################################
QColor ColorPicker_HSVCircle::get() const
{
  return d->color;
}

//##################################################################################################
void ColorPicker_HSVCircle::paintEvent(QPaintEvent *event)
{
  ColorPicker::paintEvent(event);


}

//##################################################################################################
void ColorPicker_HSVCircle::mouseMoveEvent(QMouseEvent* event)
{

}

//##################################################################################################
void ColorPicker_HSVCircle::mousePressEvent(QMouseEvent* event)
{

}

//##################################################################################################
void ColorPicker_HSVCircle::mouseReleaseEvent(QMouseEvent* event)
{

}

}

}
