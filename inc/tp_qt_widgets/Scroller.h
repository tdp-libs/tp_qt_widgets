#ifndef tp_qt_widgets_Scroller_h
#define tp_qt_widgets_Scroller_h

#include "tp_qt_widgets/Globals.h"

#include <QObject>

class QAbstractScrollArea;

namespace tp_qt_widgets
{

//##################################################################################################
//! Scroll functionality for lists on mobile devices
/*!
This class provides touch and drag scrolling for lists and scroll areas on mobile devices.
*/
class TP_QT_WIDGETS_SHARED_EXPORT Scroller: QObject
{
  Q_OBJECT
public:
  //################################################################################################
  //! Construct a new scroller and parent it to the scrollarea
  /*!
  */
  Scroller(QAbstractScrollArea* scrollArea);

  //################################################################################################
  virtual ~Scroller();

  //################################################################################################
  virtual bool eventFilter(QObject* object, QEvent* event);

  //################################################################################################
  virtual void timerEvent(QTimerEvent* event);

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
