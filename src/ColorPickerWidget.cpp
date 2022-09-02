#include "tp_qt_widgets/ColorPickerWidget.h"
#include "tp_qt_widgets/detail/ColorPicker_HSVCircle.h"
#include "tp_qt_widgets/detail/ColorPicker_HSVSquare.h"
#include "tp_qt_widgets/detail/ColorPicker_RGBSlider.h"

#include "tp_utils/RefCount.h"

#include <QBoxLayout>

namespace tp_qt_widgets
{

namespace
{

}

//##################################################################################################
struct ColorPickerWidget::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::ColorPickerWidget::Private");
  TP_NONCOPYABLE(Private);

  ColorPickerWidget* q;

  detail::ColorPicker* colorPicker;

  //################################################################################################
  Private(ColorPickerWidget* q_):
    q(q_)
  {

  }
};

//##################################################################################################
ColorPickerWidget::ColorPickerWidget(Mode mode, QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  auto l = new QVBoxLayout(this);
  l->setContentsMargins(0,0,0,0);

  switch(mode)
  {
  case Mode::HSVCircle: d->colorPicker = new detail::ColorPicker_HSVCircle(); break;
  case Mode::HSVSquare: d->colorPicker = new detail::ColorPicker_HSVSquare(); break;
  case Mode::RGBSlider: d->colorPicker = new detail::ColorPicker_RGBSlider(); break;
  }

  l->addWidget(d->colorPicker);
  connect(d->colorPicker, &detail::ColorPicker::colorChanged, this, &ColorPickerWidget::colorChanged);
}

//##################################################################################################
ColorPickerWidget::~ColorPickerWidget()
{
  delete d;
}

//##################################################################################################
void ColorPickerWidget::setColor(const QColor& color)
{
  d->colorPicker->set(color);
}

//##################################################################################################
void ColorPickerWidget::setColor(TPPixel color)
{
  QColor c;
  c.setRed  (color.r);
  c.setGreen(color.g);
  c.setBlue (color.b);
  c.setAlpha(color.a);
  d->colorPicker->set(c);
}

//##################################################################################################
QColor ColorPickerWidget::qColor() const
{
  return d->colorPicker->get();
}

//##################################################################################################
TPPixel ColorPickerWidget::tpPixel() const
{
  TPPixel color;
  {
    auto c = d->colorPicker->get();
    color.r = c.red  ();
    color.g = c.green();
    color.b = c.blue ();
    color.a = c.alpha();
  }
  return color;
}

}
