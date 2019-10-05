#ifndef tp_qt_widgets_ScrollArea_h
#define tp_qt_widgets_ScrollArea_h

#include "tp_qt_widgets/Globals.h"

#include <QScrollArea>

namespace tp_qt_widgets
{

//##################################################################################################
class TP_QT_WIDGETS_SHARED_EXPORT ScrollArea: public QScrollArea
{
public:
  //################################################################################################
  ScrollArea(QWidget* parent=nullptr);

protected:
  //################################################################################################
  virtual void resizeEvent(QResizeEvent* event);

  //################################################################################################
  virtual void showEvent(QShowEvent* event);

  //################################################################################################
  virtual bool viewportEvent(QEvent* event);

  //################################################################################################
  void enableFade(bool fade);

  //################################################################################################
  bool isFadeEnabled();

private:
  class Private;
  friend class Private;
  Private* d;
};

}
#endif
