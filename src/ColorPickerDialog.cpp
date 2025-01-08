#include "tp_qt_widgets/ColorPickerDialog.h"
#include "tp_qt_widgets/ColorPickerWidget.h"

#include "tp_utils/RefCount.h"

#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QPointer>

namespace tp_qt_widgets
{

//##################################################################################################
struct ColorPickerDialog::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::ColorPickerDialog::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  ColorPickerWidget* colorPickerWidget{nullptr};

  QSpinBox* r{nullptr};
  QSpinBox* g{nullptr};
  QSpinBox* b{nullptr};

  QSpinBox* h{nullptr};
  QSpinBox* s{nullptr};
  QSpinBox* v{nullptr};

  QLineEdit* hex{nullptr};

  //################################################################################################
  void updateUI()
  {
    auto q = colorPickerWidget->qColor();

    r->setValue(q.red  ());
    g->setValue(q.green());
    b->setValue(q.blue ());

    h->setValue(q.hsvHue       ());
    s->setValue(q.hsvSaturation());
    v->setValue(q.value        ());

    hex->setText(q.name().toUpper());
  }

  //################################################################################################
  void fromRGB()
  {
    QColor c = colorPickerWidget->qColor();

    if(c.red()==r->value() && c.green()==g->value() && c.blue()==b->value())
      return;

    colorPickerWidget->setColor(QColor(r->value(), g->value(), b->value()));
    updateUI();
  }

  //################################################################################################
  void fromHSV()
  {
    QColor c = colorPickerWidget->qColor();

    if(c.hsvHue()==h->value() && c.hsvSaturation()==s->value() && c.value()==v->value())
      return;

    colorPickerWidget->setColor(QColor::fromHsv(h->value(), s->value(), v->value()));
    updateUI();
  }

  //################################################################################################
  void fromHEX()
  {
    QString str = hex->text();

    if(!str.startsWith('#'))
      str.prepend('#');

    if(QColor c(str); c.isValid())
    {
      colorPickerWidget->setColor(c);
      updateUI();
    }
  }
};

//##################################################################################################
ColorPickerDialog::ColorPickerDialog(ColorPickerWidget::Mode mode, QWidget* parent):
  QDialog(parent),
  d(new Private())
{
  auto l = new QVBoxLayout(this);
  l->setContentsMargins(4,9,4,9);

  auto ll = new QHBoxLayout();
  ll->setContentsMargins(0,0,0,0);
  l->addLayout(ll);

  {
    auto lll = new QVBoxLayout();
    lll->setContentsMargins(0,0,0,0);
    ll->addLayout(lll);

    d->colorPickerWidget = new ColorPickerWidget(mode, false);
    lll->addWidget(d->colorPickerWidget);

    lll->addStretch();
  }

  auto lll = new QVBoxLayout();
  lll->setContentsMargins(0,0,0,0);
  ll->addLayout(lll);

  {
    lll->addWidget(new QLabel("RGB"));
    auto llll = new QHBoxLayout();
    llll->setContentsMargins(0,0,0,0);
    lll->addLayout(llll);

    d->r = new QSpinBox();
    d->r->setRange(0, 255);
    llll->addWidget(d->r);

    d->g = new QSpinBox();
    d->g->setRange(0, 255);
    llll->addWidget(d->g);

    d->b = new QSpinBox();
    d->b->setRange(0, 255);
    llll->addWidget(d->b);

    connect(d->r, &QSpinBox::editingFinished, this, [&]{d->fromRGB();});
    connect(d->g, &QSpinBox::editingFinished, this, [&]{d->fromRGB();});
    connect(d->b, &QSpinBox::editingFinished, this, [&]{d->fromRGB();});
  }

  {
    lll->addWidget(new QLabel("HSV"));
    auto llll = new QHBoxLayout();
    llll->setContentsMargins(0,0,0,0);
    lll->addLayout(llll);

    d->h = new QSpinBox();
    d->h->setRange(0, 255);
    llll->addWidget(d->h);

    d->s = new QSpinBox();
    d->s->setRange(0, 255);
    llll->addWidget(d->s);

    d->v = new QSpinBox();
    d->v->setRange(0, 255);
    llll->addWidget(d->v);

    connect(d->h, &QSpinBox::editingFinished, this, [&]{d->fromHSV();});
    connect(d->s, &QSpinBox::editingFinished, this, [&]{d->fromHSV();});
    connect(d->v, &QSpinBox::editingFinished, this, [&]{d->fromHSV();});
  }

  {
    lll->addWidget(new QLabel("HEX"));
    auto llll = new QHBoxLayout();
    llll->setContentsMargins(0,0,0,0);
    lll->addLayout(llll);

    d->hex = new QLineEdit();

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    d->hex->setFont(font);

    llll->addWidget(d->hex);

    connect(d->hex, &QLineEdit::editingFinished, this, [&]{d->fromHEX();});
  }

  lll->addStretch();

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  l->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &ColorPickerDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &ColorPickerDialog::reject);

  d->updateUI();
  connect(d->colorPickerWidget, &ColorPickerWidget::colorChanged, this, [&]{d->updateUI();});
}

//##################################################################################################
ColorPickerDialog::~ColorPickerDialog()
{
  delete d;
}

//##################################################################################################
void ColorPickerDialog::setColor(const QColor& color)
{
  d->colorPickerWidget->setColor(color);
  d->updateUI();
}

//##################################################################################################
void ColorPickerDialog::setColor(TPPixel color)
{
  d->colorPickerWidget->setColor(color);
}

//##################################################################################################
QColor ColorPickerDialog::qColor() const
{
  return d->colorPickerWidget->qColor();
}

//##################################################################################################
TPPixel ColorPickerDialog::tpPixel() const
{
  return d->colorPickerWidget->tpPixel();
}

//##################################################################################################
QColor ColorPickerDialog::getColor(const QColor& color,
                                   const QString& title,
                                   ColorPickerWidget::Mode mode,
                                   QWidget* parent)
{
  QPointer<ColorPickerDialog> dialog = new ColorPickerDialog(mode, parent);
  TP_CLEANUP([&]{delete dialog;});

  dialog->setWindowTitle(title);
  dialog->setColor(color);
  auto result = dialog->exec();

  if(!dialog || result != QDialog::Accepted)
    return QColor();

  return dialog->qColor();
}

//##################################################################################################
TPPixel ColorPickerDialog::getColor(const TPPixel& color,
                                    const QString& title,
                                    ColorPickerWidget::Mode mode,
                                    QWidget* parent)
{
  QPointer<ColorPickerDialog> dialog = new ColorPickerDialog(mode, parent);
  TP_CLEANUP([&]{delete dialog;});

  dialog->setWindowTitle(title);
  dialog->setColor(color);
  auto result = dialog->exec();

  if(!dialog || result != QDialog::Accepted)
    return TPPixel();

  return dialog->tpPixel();
}

}
