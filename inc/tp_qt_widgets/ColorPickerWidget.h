#ifndef tp_qt_widgets_ColorPickerWidget_h
#define tp_qt_widgets_ColorPickerWidget_h

#include "tp_qt_widgets/Globals.h"

#include "tp_utils/TPPixel.h"

#include <QWidget>

namespace tp_qt_widgets
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPickerWidget: public QWidget
{
  Q_OBJECT
public:
  //################################################################################################
  ColorPickerWidget(QWidget* parent=nullptr);

  //################################################################################################
  ~ColorPickerWidget() override;

  //################################################################################################
  void setColor(const QColor& color);

  //################################################################################################
  void setColor(TPPixel color);

  //################################################################################################
  template<typename T>
  void setColor(const T& color)
  {
    setColor(QColor::fromRgbF(color.x, color.y, color.z));
  }

  //################################################################################################
  QColor qColor() const;

  //################################################################################################
  TPPixel tpPixel() const;

  //################################################################################################
  template<typename T>
  T toFloat3() const
  {
    auto c = qColor();
    return T(c.redF(), c.greenF(), c.blueF());
  }

  //################################################################################################
  Q_SIGNAL void colorChanged();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
