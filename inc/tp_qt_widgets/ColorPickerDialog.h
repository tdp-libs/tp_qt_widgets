#ifndef tp_qt_Dialogs_ColorPickerDialog_h
#define tp_qt_Dialogs_ColorPickerDialog_h

#include "tp_qt_widgets/ColorPickerWidget.h"

#include "tp_utils/TPPixel.h"

#include <QDialog>

namespace tp_qt_widgets
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPickerDialog: public QDialog
{
  Q_OBJECT
  TP_DQ;
public:
  //################################################################################################
  ColorPickerDialog(ColorPickerWidget::Mode mode=ColorPickerWidget::Mode::RGBSlider, QWidget* parent=nullptr);

  //################################################################################################
  ~ColorPickerDialog() override;

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
  static QColor getColor(const QColor& color,
                         const QString& title,
                         ColorPickerWidget::Mode mode=ColorPickerWidget::Mode::RGBSlider,
                         QWidget* parent=nullptr);

  //################################################################################################
  static TPPixel getColor(const TPPixel& color,
                          const QString& title,
                          ColorPickerWidget::Mode mode=ColorPickerWidget::Mode::RGBSlider,
                          QWidget* parent=nullptr);
};

}

#endif
