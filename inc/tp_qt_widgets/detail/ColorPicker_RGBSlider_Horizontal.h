#ifndef tp_qt_widgets_ColorPicker_RGBSlider_Horizontal_h
#define tp_qt_widgets_ColorPicker_RGBSlider_Horizontal_h

#include "tp_qt_widgets/detail/ColorPicker.h"

#include <QWidget>

namespace tp_qt_widgets
{

namespace detail
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPicker_RGBSlider_Horizontal: public ColorPicker
{
  Q_OBJECT
  TP_DQ;
public:
  //################################################################################################
  ColorPicker_RGBSlider_Horizontal(QWidget* parent=nullptr);

  //################################################################################################
  ~ColorPicker_RGBSlider_Horizontal() override;

  //################################################################################################
  void set(const QColor& color) override;

  //################################################################################################
  QColor get() const override;

  //################################################################################################
  int targetHeight() const override;

protected:
  //################################################################################################
    void paintEvent(QPaintEvent *event) override;

    //##############################################################################################
    void mouseMoveEvent(QMouseEvent* event) override;

    //##############################################################################################
    void mousePressEvent(QMouseEvent* event) override;

    //##############################################################################################
    void mouseReleaseEvent(QMouseEvent* event) override;
};

}

}

#endif
