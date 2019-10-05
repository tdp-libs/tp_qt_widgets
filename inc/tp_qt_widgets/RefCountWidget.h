#ifndef tp_qt_widgets_RefCountWidget_h
#define tp_qt_widgets_RefCountWidget_h

#include "tp_qt_widgets/Globals.h"

#include <QWidget>

namespace tp_qt_widgets
{

//##################################################################################################
//! A class for counting instances of different types.
/*!
This widget displays the instance counts of all the types that inplement instance counting.
*/
class TP_QT_WIDGETS_SHARED_EXPORT RefCountWidget: public QWidget
{
public:
  //################################################################################################
  //! Construct a ref count widget.
  /*!
  This widiget will periodically update itself to display the current instance counts.
  */
  RefCountWidget(QWidget* parent=nullptr);

  //################################################################################################
  //! Destructor.
  ~RefCountWidget();


protected:
  //################################################################################################
  //! The widget is updated by a timer.
  virtual void timerEvent(QTimerEvent* event);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
