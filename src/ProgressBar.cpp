#include "tp_qt_widgets/ProgressBar.h"

#include "tp_utils/Progress.h"
#include "tp_utils/RefCount.h"

namespace tp_qt_widgets
{

//##################################################################################################
struct ProgressBar::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::ProgressBar::Private");
  TP_NONCOPYABLE(Private);
  Q* q;
  tp_utils::Progress* progress;

  //################################################################################################
  Private(Q* q_, tp_utils::Progress* progress_):
    q(q_),
    progress(progress_)
  {
    changed.connect(progress->changed);
  }

  //################################################################################################
  tp_utils::Callback<void()> changed = [&]
  {
    q->setValue(int(progress->progress() * float(q->maximum())));
  };
};

//##################################################################################################
ProgressBar::ProgressBar(tp_utils::Progress* progress, QWidget* parent):
  QProgressBar(parent),
  d(new Private(this, progress))
{
  setRange(0, 1000);
}

//##################################################################################################
ProgressBar::~ProgressBar()
{
  delete d;
}

}
