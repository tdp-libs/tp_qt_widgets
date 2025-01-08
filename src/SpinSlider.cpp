#include "tp_qt_widgets/SpinSlider.h"

#include <QDoubleSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QStyleOptionSlider>
#include <QStyle>
#include <QPainter>

namespace tp_qt_widgets
{

namespace
{
//##################################################################################################
class Slider_lt : public QSlider
{
public:
  //################################################################################################
  Slider_lt(Qt::Orientation orientation, QWidget* parent = nullptr):
    QSlider(orientation, parent)
  {

  }

  //################################################################################################
  int positionForValue(int value)
  {
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    int available = style()->pixelMetric(QStyle::PM_SliderSpaceAvailable, &opt, this);

    int sLen = style()->pixelMetric(QStyle::PM_SliderLength, &opt, this) / 2;

    int pos = style()->sliderPositionFromValue(minimum(),
                                               maximum(),
                                               value,
                                               available);

    return pos+sLen;
  }

  //################################################################################################
  void paintEvent(QPaintEvent* event) override
  {
    QSlider::paintEvent(event);

    if(softRangeSet)
    {
      QPainter painter(this);

      painter.setPen(Qt::NoPen);

      int oa=positionForValue(0);
      int ob=positionForValue(softRangeMin);
      int oc=positionForValue(softRangeMax);
      int od=positionForValue(maximum());

      painter.setBrush(QColor(255, 0, 0, 80));
      painter.drawRect(QRect(oa, 0, ob-oa, height()));
      painter.drawRect(QRect(oc, 0, od-oc, height()));

      painter.setBrush(QColor(0, 255, 0, 80));
      painter.drawRect(QRect(ob, 0, oc-ob, height()));
    }
  }

  bool softRangeSet{false};
  int softRangeMin{0};
  int softRangeMax{0};
};
}

//##################################################################################################
struct SpinSlider::Private
{
  SliderMode mode;

  QDoubleSpinBox* spinBox{nullptr};
  Slider_lt* slider{nullptr};

  bool inUpdateSpinBox{false};

  //################################################################################################
  Private(SliderMode mode_):
    mode(mode_)
  {

  }

  //################################################################################################
  void updateSliderValue()
  {
    if(inUpdateSpinBox)
      return;

    QSignalBlocker b(slider);
    slider->setValue(sliderValueFromSpinValue(spinBox->value()));
  }

  //################################################################################################
  int sliderValueFromSpinValue(double value)
  {
    double min = spinBox->minimum();
    double max = spinBox->maximum();

    double f = (value-min) / (max-min);

    if(mode == SliderMode::Exponential)
    {
      f = 1.0 - f;
      f = f*f;
      f = 1.0 - f;
    }

    f = std::clamp(f, 0.0, 1.0);

    int sliderMax = slider->maximum();

    int v = int((f*double(sliderMax))+0.5);
    return std::clamp(v, 0, sliderMax);
  }

  //################################################################################################
  double sliderValueToSpinValue(int value)
  {
    double f = double(value) / double(slider->maximum());

    if(mode == SliderMode::Exponential)
      f = 1.0 - std::sqrt(1.0 - f);

    double min = spinBox->minimum();
    double max = spinBox->maximum();

    double v = (f*(max-min))+min;
    return std::clamp(v, min, max);
  }

  //################################################################################################
  void updateSpinBoxValue()
  {
    inUpdateSpinBox = true;
    TP_CLEANUP([&]{inUpdateSpinBox = false;});


    spinBox->setValue(sliderValueToSpinValue(slider->value()));
  }
};

//##################################################################################################
SpinSlider::SpinSlider(SliderMode mode, QWidget* parent):
  QWidget(parent),
  d(new Private(mode))
{
  auto l = new QHBoxLayout(this);
  l->setContentsMargins(0,0,0,0);

  d->spinBox = new QDoubleSpinBox();
  l->addWidget(d->spinBox);

  d->slider = new Slider_lt(Qt::Horizontal);
  l->addWidget(d->slider);
  d->slider->setRange(0, 100000);

  d->updateSliderValue();

  connect(d->spinBox, &QDoubleSpinBox::valueChanged, this, [&](double value)
  {
    d->updateSliderValue();
    edited(value);
  });

  connect(d->slider, &QSlider::valueChanged, this, [&]
  {
    d->updateSpinBoxValue();
  });
}

//##################################################################################################
SpinSlider::~SpinSlider()
{
  delete d;
}

//##################################################################################################
void SpinSlider::setRange(float min, float max)
{
  d->spinBox->setRange(double(min), double(max));
  d->updateSliderValue();
}

//##################################################################################################
float SpinSlider::minValue() const
{
  return float(d->spinBox->minimum());
}

//##################################################################################################
float SpinSlider::maxValue() const
{
  return float(d->spinBox->maximum());
}

//##################################################################################################
void SpinSlider::setSoftRange(float min, float max)
{
  d->slider->softRangeSet = true;
  d->slider->softRangeMin = d->sliderValueFromSpinValue(double(min));
  d->slider->softRangeMax = d->sliderValueFromSpinValue(double(max));
  d->slider->update();
}

//##################################################################################################
void SpinSlider::setDecimals(int decimals)
{
  d->spinBox->setDecimals(decimals);
}

//##################################################################################################
int SpinSlider::decimals() const
{
  return d->spinBox->decimals();
}

//##################################################################################################
void SpinSlider::setSingleStep(float singleStep)
{
  d->spinBox->setSingleStep(double(singleStep));
}

//##################################################################################################
float SpinSlider::singleStep() const
{
  return float(d->spinBox->singleStep());
}

//##################################################################################################
void SpinSlider::setValue(float value)
{
  QSignalBlocker b(d->spinBox);
  d->spinBox->setValue(double(value));
  d->updateSliderValue();
}

//##################################################################################################
float SpinSlider::value() const
{
  return float(d->spinBox->value());
}


}

