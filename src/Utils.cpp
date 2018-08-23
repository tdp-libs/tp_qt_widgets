#include "tdp_qt_widgets/Utils.h"

#include <QFontMetricsF>

#if QT_VERSION >= 0x050000
#include <QInputMethod>
#include <QApplication>
#endif

namespace tdp_qt_widgets
{

//##################################################################################################
void setKeyboardVisible(bool visible)
{
  QApplication::inputMethod()->setVisible(visible);
}

//##################################################################################################
QRectF centerFit(float maxWidth, float maxHeight, const QRectF& source)
{
  float facA = source.width() / source.height();
  float facB = source.height() / source.width();

  float width = maxWidth;
  float height = maxWidth * facB;

  if(height>maxHeight)
  {
    width = maxHeight * facA;
    height = maxHeight;
  }

  int x = (maxWidth - width) / 2.0f;
  int y = (maxHeight - height) / 2.0f;

  return QRectF(x, y, width, height);
}

}
