#include "tp_qt_widgets/DebugWidget.h"

#include "tp_qt_utils/CrossThreadCallback.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/MutexUtils.h"

namespace tp_qt_widgets
{

//##################################################################################################
struct DebugWidget::Private
{
  DebugWidget* q;

  TPMutex mutex{TPM};
  std::string messages;

  //################################################################################################
  Private(DebugWidget* q_):
    q(q_)
  {
    // This is not thread safe but we get away with it most of the time...
    debugCallback.connect(tp_utils::DBG::Manager::instance().debugCallbacks);
  }

  //################################################################################################
  tp_qt_utils::CrossThreadCallback addMessages = std::function<void()>([&]
  {
    TP_MUTEX_LOCKER(mutex);
    q->setPlainText(q->toPlainText() + QString::fromStdString(messages));
    messages.clear();
  });

  //################################################################################################
  tp_utils::Callback<void(tp_utils::MessageType, const std::string&)> debugCallback = [&](tp_utils::MessageType, const std::string& msg)
  {
    TP_MUTEX_LOCKER(mutex);
    messages+=msg;
    addMessages.call();
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
