#ifndef tp_qt_widgets_ColorPicker_h
#define tp_qt_widgets_ColorPicker_h

#include "tp_qt_widgets/Globals.h"

#include <QWidget>

namespace tp_qt_widgets
{

namespace detail
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPicker: public QWidget
{
  Q_OBJECT
public:
  //################################################################################################
  ColorPicker(QWidget* parent=nullptr);

  //################################################################################################
  virtual void set(const QColor& color)=0;

  //################################################################################################
  virtual QColor get() const=0;

  //################################################################################################
  bool bestContrastIsBlack();

  //################################################################################################
  //! Components: 'r', 'g', 'b', 'h', 's', 'v'
  double getComponentF(char component);

  //################################################################################################
  Q_SIGNAL void colorChanged();

  //################################################################################################
  QSize minimumSizeHint() const override;

  //################################################################################################
  QSize sizeHint() const override;

  //################################################################################################
  bool hasHeightForWidth() const override;

  //################################################################################################
  int heightForWidth(int w) const override;

};

}

}

#endif
