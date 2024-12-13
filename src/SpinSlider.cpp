#include "tp_qt_widgets/SpinSlider.h"

#include <QDoubleSpinBox>
#include <QSlider>
#include <QBoxLayout>

namespace tp_qt_widgets
{

//##################################################################################################
struct SpinSlider::Private
{
  QDoubleSpinBox* spinBox{nullptr};
  QSlider* slider{nullptr};

  bool inUpdateSpinBox{false};

  //################################################################################################
  void updateSliderValue()
  {
    if(inUpdateSpinBox)
      return;

    slider->blockSignals(true);
    TP_CLEANUP([&]{slider->blockSignals(false);});

    double min = spinBox->minimum();
    double max = spinBox->maximum();

    double value = spinBox->value();

    double f = (value-min) / (max-min);
    f = std::clamp(f, 0.0, 1.0);

    int sliderMax = slider->maximum();

    int v = int((f*double(sliderMax))+0.5);
    v = std::clamp(v, 0, sliderMax);

    slider->setValue(v);
  }

  //################################################################################################
  void updateSpinBoxValue()
  {
    inUpdateSpinBox = true;
    TP_CLEANUP([&]{inUpdateSpinBox = false;});

    double f = double(slider->value()) / double(slider->maximum());

    double min = spinBox->minimum();
    double max = spinBox->maximum();

    double v = (f*(max-min))+min;
    v = std::clamp(v, min, max);

    spinBox->setValue(v);
  }
};

//##################################################################################################
SpinSlider::SpinSlider(QWidget* parent):
  QWidget(parent),
  d(new Private())
{
  auto l = new QHBoxLayout(this);
  l->setContentsMargins(0,0,0,0);

  d->spinBox = new QDoubleSpinBox();
  l->addWidget(d->spinBox);

  d->slider = new QSlider(Qt::Horizontal);
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
void SpinSlider::setValue(float value)
{
  d->spinBox->setValue(double(value));
  d->updateSliderValue();
}

//##################################################################################################
float SpinSlider::value() const
{
  return float(d->spinBox->value());
}


}

