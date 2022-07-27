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
  CollapsiblePanel* q;
  QScrollArea* contentArea{nullptr};

  QVariantAnimation* contentAnimation{nullptr};

  float f{1.0f};

  //################################################################################################
  Private(CollapsiblePanel* q_):
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
      updateGeometry();
    });

    contentAnimation->start();
  }

  //################################################################################################
  void updateGeometry()
  {
    float height = float(contentArea->layout()->sizeHint().height()) * f;

    if(contentArea->layout()->expandingDirections() & Qt::Orientation::Vertical)
      height*=2;

    height += 0.5f;

    q->setMaximumHeight(int(height));
    q->updateGeometry();
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
  d->contentArea->installEventFilter(this);
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

//##################################################################################################
bool CollapsiblePanel::eventFilter(QObject* watched, QEvent* event)
{
  TP_UNUSED(watched);

  if(event->type() == QEvent::LayoutRequest)
    d->updateGeometry();

  return false;
}
}
