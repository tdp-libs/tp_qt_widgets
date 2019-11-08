#ifndef tp_qt_Dialogs_ColorPickerDialog_h
#define tp_qt_Dialogs_ColorPickerDialog_h

#include "tp_qt_widgets/Globals.h"

#include <QDialog>

namespace tp_qt_widgets
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT ColorPickerDialog: public QDialog
{
  Q_OBJECT
public:
  //################################################################################################
  ColorPickerDialog(QDialog* parent=nullptr);

  //################################################################################################
  ~ColorPickerDialog() override;

  //################################################################################################
  void setColor(const QColor& color);

  //################################################################################################
  QColor color() const;

  //################################################################################################
  static QColor getColor(const QColor& color, const QString& title, QDialog* parent=nullptr);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
