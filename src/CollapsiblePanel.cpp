#include "tp_qt_widgets/CollapsiblePanel.h"

#include <QScrollArea>
#include <QVariantAnimation>
#include <QBoxLayout>
#include <QEvent>

namespace tp_qt_widgets
{

//##################################################################################################
struct CollapsiblePanel::Private
{
  Q* q;
  QScrollArea* contentArea{nullptr};

  QVariantAnimation* contentAnimation{nullptr};

  float f{1.0f};

  bool expanded{true};

  //################################################################################################
  Private(Q* q_):
    q(q_)
  {

  }

  //################################################################################################
  void makeAnimation(float start, float end, int duration)
  {
    delete contentAnimation;
    contentAnimation = new QVariantAnimation(q);
    contentAnimation->setDuration(duration);
    contentAnimation->setStartValue(start);
    contentAnimation->setEndValue(end);

    QObject::connect(contentAnimation, &QVariantAnimation::valueChanged, q, [&](const QVariant& v)
    {
      f = v.toFloat();
      q->recalculateGeometry();
      emit q->expansionFactorChanged();
    });

    contentAnimation->start();
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
  d->contentArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  l->addWidget(d->contentArea, 1);
  d->contentArea->installEventFilter(this);
  d->contentArea->setFrameStyle(QFrame::NoFrame);

  recalculateGeometry();
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
QLayout* CollapsiblePanel::contentLayout() const
{
  return d->contentArea->layout();
}

//##################################################################################################
void CollapsiblePanel::expand()
{
  if(d->expanded)
    return;

  d->expanded = true;
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);


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

  if(duration<1)
    duration = 10;

  d->makeAnimation(start, end, duration);
}

//##################################################################################################
void CollapsiblePanel::collapse()
{
  if(!d->expanded)
    return;

  d->expanded = false;
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

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

  if(duration<1)
    duration = 10;

  d->makeAnimation(start, end, duration);
}

//##################################################################################################
void CollapsiblePanel::collapseNow()
{
  d->expanded = false;
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  d->f = 0.0f;
  recalculateGeometry();
}

//##################################################################################################
float CollapsiblePanel::expansionFactor() const
{
  return d->f;
}

//##################################################################################################
void CollapsiblePanel::recalculateGeometry()
{
  float height = 1000.0f;
  if(auto w=parentWidget(); w)
    height=w->height();

  height*=d->f;

  setMaximumHeight(int(height));
  updateGeometry();
}

//################################################################################################
void CollapsiblePanel::showEvent(QShowEvent* event)
{
  recalculateGeometry();
  QWidget::showEvent(event);
}

//##################################################################################################
bool CollapsiblePanel::eventFilter(QObject* watched, QEvent* event)
{
  TP_UNUSED(watched);

  if(event->type() == QEvent::LayoutRequest)
    recalculateGeometry();

  return false;
}
}
