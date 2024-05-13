#include "tp_qt_widgets/ColorButton.h"
#include "tp_qt_widgets/ColorPickerDialog.h"

#include <QPainter>

namespace tp_qt_widgets
{

//##################################################################################################
struct ColorButton::Private
{
  Q* q;
  QColor color;

  //################################################################################################
  Private(Q* q_):
    q(q_)
  {

  }

  //################################################################################################
  void updateIcon()
  {
    QImage image(24, 24, QImage::Format_ARGB32);
    image.fill(QColor(0,0,0,0));
    {
      QPainter p(&image);
      p.setBrush(color);
      p.setPen(Qt::black);
      p.drawRoundedRect(2,2,20,20,2.0, 2.0);
    }
    q->setIcon(QIcon(QPixmap::fromImage(image)));
  }
};

//##################################################################################################
ColorButton::ColorButton(const QString& text, ColorPickerWidget::Mode mode, QWidget* parent):
  QPushButton(text, parent),
  d(new Private(this))
{
  setStyleSheet("text-align:left; padding-left:2;");

  connect(this, &QAbstractButton::clicked, this, [=]
  {
    QColor color = ColorPickerDialog::getColor(d->color, text, mode, this);
    if(color.isValid())
    {
      setColor(color);
      edited();
    }
  });

  setColor(QColor(120, 120, 200));
}

//##################################################################################################
ColorButton::~ColorButton()
{
  delete d;
}

//##################################################################################################
void ColorButton::setColor(const QColor& color)
{
  d->color = color;
  d->updateIcon();
}

//##################################################################################################
void ColorButton::setColor(TPPixel color)
{
  setColor(tpPixelToQColor(color));
}

//##################################################################################################
QColor ColorButton::qColor() const
{
  return d->color;
}

//##################################################################################################
TPPixel ColorButton::tpPixel() const
{
  return tpPixelFromQColor(qColor());
}

}
