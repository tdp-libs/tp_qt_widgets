#ifndef tp_qt_widgets_ColorPickerWidget_h
#define tp_qt_widgets_ColorPickerWidget_h

#include "tp_qt_widgets/Globals.h"

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
  QColor color() const;

  //################################################################################################
  Q_SIGNAL void colorChanged();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
