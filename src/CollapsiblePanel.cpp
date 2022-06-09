#include "tp_qt_widgets/CollapsiblePanel.h"

#include <QScrollArea>
#include <QPropertyAnimation>
#include <QBoxLayout>

namespace tp_qt_widgets
{

//##################################################################################################
struct CollapsiblePanel::Private
{
  CollapsiblePanel* q;
  QScrollArea* contentArea{nullptr};

  QPropertyAnimation* contentAnimation{nullptr};

  //################################################################################################
  Private(CollapsiblePanel* q_):
    q(q_)
  {

  }

  //################################################################################################
  void makeAnimation(float start, float end, int duration)
  {
    delete contentAnimation;
    contentAnimation = new QPropertyAnimation(q);
    contentAnimation->setDuration(duration);
    contentAnimation->setStartValue(start);
    contentAnimation->setEndValue(end);
  }
};

//##################################################################################################
CollapsiblePanel::CollapsiblePanel(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  auto l = new QVBoxLayout(this);
  l->setContentsMargins(0,0,0,0);

  d->contentArea = new QScrollArea();
  l->addWidget(d->contentArea);
}

//##################################################################################################
CollapsiblePanel::~CollapsiblePanel()
{
  delete d;
}

//##################################################################################################
void CollapsiblePanel::setContentLayout(QLayout* layout)
{
  delete d->contentArea->layout();
  d->contentArea->setLayout(layout);
}

//##################################################################################################
void CollapsiblePanel::expand()
{
  int duration = 300;
  float start = 0.0f;
  float end = 1.0f;
  if(d->contentAnimation)
  {
    if(d->contentAnimation->endValue().toFloat()>0.9f)
      return;

    start = d->contentAnimation->currentValue().toFloat();
    duration -= (duration - d->contentAnimation->currentTime());
  }

  d->makeAnimation(start, end, duration);
}

//##################################################################################################
void CollapsiblePanel::collapse()
{
  int duration = 300;
  float start = 1.0f;
  float end = 0.0f;
  if(d->contentAnimation)
  {
    if(d->contentAnimation->endValue().toFloat()<0.1f)
      return;

    start = d->contentAnimation->currentValue().toFloat();
    duration -= (duration - d->contentAnimation->currentTime());
  }

  d->makeAnimation(start, end, duration);
}

}
