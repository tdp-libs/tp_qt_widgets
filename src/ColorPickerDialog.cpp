#include "tp_qt_widgets/ColorPickerDialog.h"
#include "tp_qt_widgets/ColorPickerWidget.h"

#include <QDialogButtonBox>
#include <QBoxLayout>
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
};

//##################################################################################################
ColorPickerDialog::ColorPickerDialog(QDialog* parent):
  QDialog(parent),
  d(new Private())
{
  auto l = new QVBoxLayout(this);
  l->setContentsMargins(4,9,4,9);

  d->colorPickerWidget = new ColorPickerWidget();
  l->addWidget(d->colorPickerWidget);

  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  l->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &ColorPickerDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &ColorPickerDialog::reject);
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
QColor ColorPickerDialog::getColor(const QColor& color, const QString& title, QDialog* parent)
{
  QPointer<ColorPickerDialog> dialog = new ColorPickerDialog(parent);
  TP_CLEANUP([&]{delete dialog;});

  dialog->setWindowTitle(title);
  dialog->setColor(color);
  auto result = dialog->exec();

  if(!dialog || result != QDialog::Accepted)
    return QColor();

  return dialog->qColor();
}

//##################################################################################################
TPPixel ColorPickerDialog::getColor(const TPPixel& color, const QString& title, QDialog* parent)
{
  QPointer<ColorPickerDialog> dialog = new ColorPickerDialog(parent);
  TP_CLEANUP([&]{delete dialog;});

  dialog->setWindowTitle(title);
  dialog->setColor(color);
  auto result = dialog->exec();

  if(!dialog || result != QDialog::Accepted)
    return TPPixel();

  return dialog->tpPixel();
}

}
