#include "tp_qt_widgets/ColorPickerWidget.h"

#include <QBoxLayout>
#include <QSlider>
#include <QFrame>

namespace tp_qt_widgets
{

//##################################################################################################
struct ColorPickerWidget::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::ColorPickerWidget::Private");
  TP_NONCOPYABLE(Private);

  ColorPickerWidget* q;

  QColor color{128, 255, 128};

  QSlider* rSlider{nullptr};
  QSlider* gSlider{nullptr};
  QSlider* bSlider{nullptr};

  QFrame* preview{nullptr};

  //################################################################################################
  Private(ColorPickerWidget* q_):
    q(q_)
  {

  }

  //################################################################################################
  void updatePreview()
  {
    preview->setStyleSheet(QString("background-color: %1;").arg(color.name()));
  }

  //################################################################################################
  void updateUI()
  {
    rSlider->blockSignals(true);
    gSlider->blockSignals(true);
    bSlider->blockSignals(true);

    rSlider->setValue(color.red  ());
    gSlider->setValue(color.green());
    bSlider->setValue(color.blue ());

    rSlider->blockSignals(false);
    gSlider->blockSignals(false);
    bSlider->blockSignals(false);

    updatePreview();
  }

  //################################################################################################
  void updateColor()
  {
    color = {rSlider->value(), gSlider->value(), bSlider->value()};
    updatePreview();
    Q_EMIT q->colorChanged();
  }
};

//##################################################################################################
ColorPickerWidget::ColorPickerWidget(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  auto hLayout = new QHBoxLayout(this);
  hLayout->setContentsMargins(0,0,0,0);

  {
    auto sliderLayout = new QVBoxLayout();

    d->rSlider = new QSlider(Qt::Horizontal);
    d->gSlider = new QSlider(Qt::Horizontal);
    d->bSlider = new QSlider(Qt::Horizontal);

    d->rSlider->setRange(0, 255);
    d->gSlider->setRange(0, 255);
    d->bSlider->setRange(0, 255);

    sliderLayout->addWidget(d->rSlider);
    sliderLayout->addWidget(d->gSlider);
    sliderLayout->addWidget(d->bSlider);

    connect(d->rSlider, &QSlider::sliderMoved, this, [&]{d->updateColor();});
    connect(d->gSlider, &QSlider::sliderMoved, this, [&]{d->updateColor();});
    connect(d->bSlider, &QSlider::sliderMoved, this, [&]{d->updateColor();});

    connect(d->rSlider, &QSlider::sliderReleased, this, [&]{d->updateColor();});
    connect(d->gSlider, &QSlider::sliderReleased, this, [&]{d->updateColor();});
    connect(d->bSlider, &QSlider::sliderReleased, this, [&]{d->updateColor();});

    connect(d->rSlider, &QSlider::valueChanged, this, [&]{d->updateColor();});
    connect(d->gSlider, &QSlider::valueChanged, this, [&]{d->updateColor();});
    connect(d->bSlider, &QSlider::valueChanged, this, [&]{d->updateColor();});

    hLayout->addLayout(sliderLayout);
  }

  d->preview = new QFrame();
  hLayout->addWidget(d->preview);
  d->preview->setFixedSize(100, 100);
  d->preview->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  d->updateUI();
}

//##################################################################################################
ColorPickerWidget::~ColorPickerWidget()
{
  delete d;
}

//##################################################################################################
void ColorPickerWidget::setColor(const QColor& color)
{
  d->color = color;
  d->updateUI();
}

//##################################################################################################
void ColorPickerWidget::setColor(TPPixel color)
{
  d->color.setRed  (color.r);
  d->color.setGreen(color.g);
  d->color.setBlue (color.b);
  d->color.setAlpha(color.a);
  d->updateUI();
}

//##################################################################################################
QColor ColorPickerWidget::qColor() const
{
  return d->color;
}

//##################################################################################################
TPPixel ColorPickerWidget::tpPixel() const
{
  TPPixel color;
  color.r = d->color.red  ();
  color.g = d->color.green();
  color.b = d->color.blue ();
  color.a = d->color.alpha();
  return color;
}

}
