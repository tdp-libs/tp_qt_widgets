#pragma once

#include "tp_qt_widgets/Globals.h"

#include <QScrollArea>

namespace tp_qt_widgets
{

//##################################################################################################
//! A color picker
class TP_QT_WIDGETS_SHARED_EXPORT WheelSafeScrollArea : public QScrollArea
{
  Q_OBJECT
  TP_DQ;
public:

  //################################################################################################
  WheelSafeScrollArea();

  //################################################################################################
  ~WheelSafeScrollArea() override;

  //################################################################################################
  bool eventFilter(QObject* watched, QEvent* event) override;

  //################################################################################################
  void updateWatchedObjects();

private:
  //################################################################################################
  Q_SLOT void childDestroyed(QObject* object);

protected:
  //################################################################################################
  void showEvent(QShowEvent* event) override;
};

}
