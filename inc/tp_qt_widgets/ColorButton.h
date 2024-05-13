#ifndef tp_qt_widgets_ColorButton_h
#define tp_qt_widgets_ColorButton_h

#include "tp_qt_widgets/ColorPickerWidget.h"

#include "tp_utils/TPPixel.h"

#include <QPushButton>

namespace tp_qt_widgets
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorButton: public QPushButton
{
  Q_OBJECT
  TP_DQ;
public:
  //################################################################################################
  ColorButton(const QString& text,
              ColorPickerWidget::Mode mode=ColorPickerWidget::Mode::RGBSlider,
              QWidget* parent=nullptr);

  //################################################################################################
  ~ColorButton() override;

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
  tp_utils::CallbackCollection<void()> edited;
};

}

#endif
