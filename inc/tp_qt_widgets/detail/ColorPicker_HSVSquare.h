#ifndef tp_qt_widgets_ColorPicker_HSVSquare_h
#define tp_qt_widgets_ColorPicker_HSVSquare_h

#include "tp_qt_widgets/detail/ColorPicker.h"

#include <QWidget>

namespace tp_qt_widgets
{

namespace detail
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPicker_HSVSquare: public ColorPicker
{
  Q_OBJECT
public:
  //################################################################################################
  ColorPicker_HSVSquare(QWidget* parent=nullptr);

  //################################################################################################
  ~ColorPicker_HSVSquare() override;

  //################################################################################################
  void set(const QColor& color) override;

  //################################################################################################
  QColor get() const override;

protected:
  //################################################################################################
    void paintEvent(QPaintEvent *event) override;

    //##############################################################################################
    void mouseMoveEvent(QMouseEvent* event) override;

    //##############################################################################################
    void mousePressEvent(QMouseEvent* event) override;

    //##############################################################################################
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

}

#endif
