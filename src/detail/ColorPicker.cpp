#include "tp_qt_widgets/detail/ColorPicker.h"

#include "tp_utils/TPPixel.h"

namespace tp_qt_widgets
{

namespace detail
{

//##################################################################################################
ColorPicker::ColorPicker(QWidget* parent):
  QWidget(parent)
{

}

//##################################################################################################
bool ColorPicker::bestContrastIsBlack()
{
  auto c = get();
  TPPixel p;
  p.r = c.red();
  p.g = c.green();
  p.b = c.blue();
  return p.bestContrastIsBlack();
}

//##################################################################################################
double ColorPicker::getComponentF(char component)
{
  switch(component)
  {
  case 'r': return double(get().  redF());
  case 'g': return double(get().greenF());
  case 'b': return double(get(). blueF());
  }
  return 0.5;
}

//##################################################################################################
QSize ColorPicker::minimumSizeHint() const
{
  return QSize(200, 100);
}

//##################################################################################################
QSize ColorPicker::sizeHint() const
{
  return QSize(300, 200);
}

//##################################################################################################
bool ColorPicker::hasHeightForWidth() const
{
  return true;
}

//##################################################################################################
int ColorPicker::heightForWidth(int w) const
{
  return int((float(w)/3.0f)*2.0f);
}

}

}
