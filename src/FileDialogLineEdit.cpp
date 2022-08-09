#include "tp_qt_widgets/FileDialogLineEdit.h"

#include "tp_utils/RefCount.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>

namespace tp_qt_widgets
{
//##################################################################################################
struct FileDialogLineEdit::Private
{
  TP_REF_COUNT_OBJECTS("tp_qt_widgets::FileDialogLineEdit::Private");
  TP_NONCOPYABLE(Private);

  FileDialogLineEdit* q;

  QLineEdit* lineEdit{nullptr};
  QPushButton* pushButton{nullptr};

  QString initialDirectory;
  QString filter;
  FileDialogLineEdit::Mode mode{FileDialogLineEdit::DirectoryMode};

  QString qSettingsPath;

  //################################################################################################
  Private(FileDialogLineEdit* q_):
    q(q_)
  {

  }

  //################################################################################################
  QString dir()
  {
    if(lineEdit->text().isEmpty())
      return initialDirectory;

    if(mode == FileDialogLineEdit::DirectoryMode)
      return lineEdit->text();

    QStringList parts = lineEdit->text().split("/");
    parts.removeLast();
    return parts.join('/');
  }
};

//##################################################################################################
FileDialogLineEdit::FileDialogLineEdit(QWidget* parent):
  QWidget(parent),
  d(new Private(this))
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  d->lineEdit = new QLineEdit();
  layout->addWidget(d->lineEdit);

  d->pushButton = new QPushButton("Select");
  d->pushButton->setFixedWidth(100);

  connect(d->pushButton, &QPushButton::clicked, this, [&]()
  {
    QString str;

    if(d->mode == FileDialogLineEdit::DirectoryMode)
      str = QFileDialog::getExistingDirectory(d->q->parentWidget(), "Select Directory", d->dir());
    else if(d->mode == FileDialogLineEdit::OpenFileMode)
      str = QFileDialog::getOpenFileName(d->q->parentWidget(), "Load File", d->dir(), d->filter);
    else
      str = QFileDialog::getSaveFileName(d->q->parentWidget(), "Save File", d->dir(), d->filter);

    if(!str.isEmpty())
    {
      d->lineEdit->setText(str);

      if(!d->qSettingsPath.isEmpty())
        QSettings().setValue(d->qSettingsPath, d->dir());

      Q_EMIT d->q->selectionChanged();
    }
  });

  layout->addWidget(d->pushButton);
}

//##################################################################################################
FileDialogLineEdit::~FileDialogLineEdit()
{
  delete d;
}

//##################################################################################################
void FileDialogLineEdit::setText(const QString& text)
{
  d->lineEdit->setText(text);
}

//##################################################################################################
QString FileDialogLineEdit::text() const
{
  return d->lineEdit->text();
}

//##################################################################################################
void FileDialogLineEdit::setInitialDirectory(const QString& dir)
{
  d->initialDirectory = dir;
}

//##################################################################################################
void FileDialogLineEdit::setMode(FileDialogLineEdit::Mode mode)
{
  d->mode = mode;
}

//##################################################################################################
void FileDialogLineEdit::setFilter(const QString& filter)
{
  d->filter = filter;
}

//##################################################################################################
void FileDialogLineEdit::setQSettingsPath(const QString& qSettingsPath)
{
  d->qSettingsPath = qSettingsPath;
  if(!d->qSettingsPath.isEmpty())
    d->initialDirectory = QSettings().value(d->qSettingsPath, d->initialDirectory).toString();
}


}
