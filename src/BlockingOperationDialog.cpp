
#include "tp_qt_widgets/BlockingOperationDialog.h"
#include "tp_qt_widgets/ProgressBar.h"

#include <QPointer>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QScrollBar>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>
#include <QSettings>

namespace tp_qt_widgets
{

//##################################################################################################
struct BlockingOperationDialog::Private
{
  std::function<bool()> poll;
  bool finish{false};
  bool ok=false;
  bool allowClose=false;

  //QPlainTextEdit* messages{nullptr};
  QTextEdit* messages{nullptr};
  tp_qt_widgets::ProgressBar* progressBar{nullptr};
  QCheckBox* keepOpen{nullptr};
  QDialogButtonBox* buttons{nullptr};

  //################################################################################################
  Private(const std::function<bool()>& poll_):
    poll(poll_),
    progress(poll)
  {
    changed.connect(progress.changed);
  }

  //################################################################################################
  tp_utils::Progress progress = tp_utils::Progress([&]
  {
    if(!poll())
      return false;
    return !finish;
  });

  //################################################################################################
  tp_utils::Callback<void()> changed = [&]
  {
    poll();

    bool atBottom = (messages->verticalScrollBar()->maximum()-messages->verticalScrollBar()->value()) < 20;
    int current = messages->verticalScrollBar()->value();

    {
      TP_CLEANUP(tp_qt_widgets::keepTextSelection(messages));

      messages->clear();
      std::string text;
      for(const auto& message : progress.allMessages())
      {
        size_t iMax = message.indentation*4;
        for(size_t i=0; i<iMax; i++)
        text += "&nbsp;";
        if(message.error)
          text += "<font color=#8b0000><b>" + message.message + "</b></font>";
        else
          text += message.message;
        text += "<br>";
      }

      messages->setHtml(QString::fromStdString(text));
    }

    if(atBottom)
      messages->verticalScrollBar()->setValue(messages->verticalScrollBar()->maximum());
    else
      messages->verticalScrollBar()->setValue(current);

    poll();
  };
};

//##################################################################################################
BlockingOperationDialog::BlockingOperationDialog(const std::function<bool()>& poll, QWidget* parent):
  QDialog(parent),
  d(new Private(poll))
{
  auto l = new QVBoxLayout(this);

  d->progressBar = new tp_qt_widgets::ProgressBar(&d->progress);
  l->addWidget(d->progressBar);

  d->messages = new QTextEdit();
  l->addWidget(d->messages);
  d->messages->setReadOnly(true);
  d->messages->setWordWrapMode(QTextOption::NoWrap);

  d->keepOpen = new QCheckBox("Keep open");
  d->keepOpen->setChecked(QSettings().value("BlockingOperationDialog_keepOpen").toBool());
  l->addWidget(d->keepOpen);

  d->buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
  l->addWidget(d->buttons);
  connect(d->buttons, &QDialogButtonBox::rejected, this, [=]
  {
    d->progress.addError("Canceled!");
    d->progress.stop(true);
    d->finish=true;
  });
}

//##################################################################################################
BlockingOperationDialog::~BlockingOperationDialog()
{
  QSettings().setValue("BlockingOperationDialog_keepOpen", d->keepOpen->isChecked());
  delete d;
}

//##################################################################################################
bool BlockingOperationDialog::exec(const std::function<bool()>& poll,
                                   const QString& windowTitle,
                                   QWidget* parent,
                                   const std::function<bool(tp_utils::Progress*)>& closure)

{
  return exec(poll, windowTitle, parent, [&](QWidget*, tp_utils::Progress* progress){return closure(progress);});
}

//##################################################################################################
bool BlockingOperationDialog::exec(const std::function<bool()>& poll,
                                   const QString& windowTitle,
                                   QWidget* parent,
                                   const std::function<bool(QWidget* parent, tp_utils::Progress* progress)>& closure)
{
  QPointer<BlockingOperationDialog> dialog = new BlockingOperationDialog(poll, parent);
  TP_CLEANUP([&]{delete dialog;});
  dialog->setWindowTitle(windowTitle);
  dialog->setFixedSize(600, 480);

  auto timer = new QTimer(dialog);
  timer->setSingleShot(true);
  timer->start(0);
  connect(timer, &QTimer::timeout, dialog, [&]
  {
    auto showCloseButton = [&]
    {
      dialog->d->allowClose = true;
      dialog->d->buttons->setStandardButtons(QDialogButtonBox::Close);
      dialog->d->buttons->disconnect(dialog->d->buttons);
      connect(dialog->d->buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
      connect(dialog->d->buttons, &QDialogButtonBox::rejected, dialog, &QDialog::accept);
    };

    bool ok = closure(dialog.data(), &dialog->d->progress);

    if(!dialog)
      return;

    dialog->d->ok = ok;

    if(dialog->d->ok)
    {
      if(dialog)
      {
        dialog->d->progress.setProgress(1.0f);
        QPalette p = dialog->d->messages->palette();
        p.setColor(QPalette::Base, QColor(201, 255, 216));
        dialog->d->messages->setPalette(p);
        if(!dialog->d->keepOpen->isChecked())
          dialog->accept();
        else
          showCloseButton();
      }
    }
    else
    {
      QPalette p = dialog->d->messages->palette();
      p.setColor(QPalette::Base, QColor(255, 189, 189));
      dialog->d->messages->setPalette(p);
      QPointer<QDialog> errorDialog = new QDialog(dialog);

      errorDialog->setWindowTitle("Error!");

      auto l = new QVBoxLayout(errorDialog);
      l->addWidget(new QLabel("Error details."));

      auto text = new QPlainTextEdit();
      text->setWordWrapMode(QTextOption::NoWrap);
      text->setPlainText(QString::fromStdString(dialog->d->progress.compileErrors()));
      l->addWidget(text);

      auto buttons = new QDialogButtonBox(QDialogButtonBox::Close);
      l->addWidget(buttons);

      connect(buttons, &QDialogButtonBox::accepted, errorDialog, &QDialog::accept);
      connect(buttons, &QDialogButtonBox::rejected, errorDialog, &QDialog::reject);

      auto errorTimer = new QTimer(errorDialog);
      errorTimer->setSingleShot(true);
      errorTimer->start(0);
      connect(errorTimer, &QTimer::timeout, dialog, [=]
      {
        errorDialog->exec();

        if(dialog)
        {
          if(ok && !dialog->d->keepOpen->isChecked())
            dialog->reject();
          else
            showCloseButton();
        }
      });
    }
  });

  if(dialog.data()->QDialog::exec() != QDialog::Accepted)
    return false;

  return dialog?dialog->d->ok:false;
}

//##################################################################################################
void BlockingOperationDialog::keyPressEvent(QKeyEvent* event)
{
  if(d->allowClose)
    return QDialog::keyPressEvent(event);

  if(event->key() == Qt::Key_Escape)
    event->ignore();
}

//##################################################################################################
void BlockingOperationDialog::closeEvent(QCloseEvent* event)
{
  if(d->allowClose)
    return QDialog::closeEvent(event);

  event->ignore();
}

}
