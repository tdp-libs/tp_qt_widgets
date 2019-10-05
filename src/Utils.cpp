#include "tp_qt_widgets/Utils.h"

#include <QFontMetricsF>

#if QT_VERSION >= 0x050000
#include <QInputMethod>
#include <QApplication>
#endif

namespace tp_qt_widgets
{

//##################################################################################################
void setKeyboardVisible(bool visible)
{
  QApplication::inputMethod()->setVisible(visible);
}

}
