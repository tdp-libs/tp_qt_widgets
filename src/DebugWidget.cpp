#include "tp_qt_widgets/DebugWidget.h"

#include "tp_utils/DebugUtils.h"

namespace tp_qt_widgets
{

//##################################################################################################
struct DebugWidget::Private
{
  DebugWidget* q;

  //################################################################################################
  Private(DebugWidget* q_):
    q(q_)
  {
    debugCallback.connect(tp_utils::DBG::Manager::instance().debugCallbacks);
  }

  //################################################################################################
  tp_utils::Callback<void(tp_utils::MessageType, const std::string&)> debugCallback = [&](tp_utils::MessageType, const std::string& msg)
  {
    q->setPlainText(q->toPlainText() + QString::fromStdString(msg));
  };
};

//##################################################################################################
DebugWidget::DebugWidget(QWidget* parent):
  QPlainTextEdit(parent),
  d(new Private(this))
{

}

//##################################################################################################
DebugWidget::~DebugWidget()
{
  delete d;
}

}
