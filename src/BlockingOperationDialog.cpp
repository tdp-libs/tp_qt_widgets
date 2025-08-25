#include "tp_qt_widgets/BlockingOperationDialog.h"
#include "tp_qt_widgets/ProgressBar.h"

#include "tp_utils/TPSettings.h"

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

namespace tp_qt_widgets
{

//##################################################################################################
struct BlockingOperationDialog::Private
{
  std::function<bool()> poll;
  tp_utils::Progress* progress{nullptr};
  std::string windowTitle;
  bool finish{false};
  bool ok=false;
  bool allowClose=false;

  QTextEdit* messages{nullptr};
  tp_qt_widgets::ProgressBar* progressBar{nullptr};
  QCheckBox* keepOpen{nullptr};
  QDialogButtonBox* buttons{nullptr};
  QBoxLayout* buttonLayout{nullptr};

  //################################################################################################
  Private(const std::function<bool()>& poll_, const QString& windowTitle_):
    poll(poll_),
    windowTitle(windowTitle_.toStdString())
  {
    progress = &ourPogress;
    changed.connect(progress->changed);
  }

  //################################################################################################
  Private(tp_utils::Progress* progress_, const QString& windowTitle_):
    progress(progress_),
    windowTitle(windowTitle_.toStdString())
  {
    poll = [this]
    {
      if(!progress->poll())
        return false;
      return !finish;
    };

    changed.connect(progress->changed);
  }

  //################################################################################################
  tp_utils::Progress ourPogress = tp_utils::Progress([&]
  {
    if(!poll())
      return false;
    return !finish;
  }, windowTitle);

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

      {
        const auto& messages = progress->allMessages();
        size_t maxRows = 500;
        size_t iMax = messages.size();
        size_t iFirst = (iMax>maxRows)?(iMax-maxRows):0;
        for(size_t i=iFirst; i<iMax; i++)
        {
          const auto& message = messages.at(i);
          size_t iMax = message.indentation*4;
          for(size_t i=0; i<iMax; i++)
            text += "&nbsp;";
          if(message.error)
            text += "<font color=#8b0000><b>" + message.message + "</b></font>";
          else
            text += message.message;
          text += "<br>";
        }
      }

      messages->setHtml(QString::fromStdString(text));
    }

    if(atBottom)
      messages->verticalScrollBar()->setValue(messages->verticalScrollBar()->maximum());
    else
      messages->verticalScrollBar()->setValue(current);

    poll();
  };

  //################################################################################################
  bool shouldKeepOpen()
  {
    return keepOpen->isChecked() && !keepOpen->isHidden();
  }

  //################################################################################################
  void createUI(BlockingOperationDialog* q, const QString& windowTitle)
  {
    q->setWindowTitle(windowTitle);
    auto l = new QVBoxLayout(q);

    progressBar = new tp_qt_widgets::ProgressBar(progress);
    l->addWidget(progressBar);

    messages = new QTextEdit();
    l->addWidget(messages);
    messages->setReadOnly(true);
    messages->setWordWrapMode(QTextOption::NoWrap);

    keepOpen = new QCheckBox("Keep open");
    keepOpen->setChecked(TPSettings::value("BlockingOperationDialog_keepOpen")=="true");
    l->addWidget(keepOpen);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0,0,0,0);
    l->addLayout(buttonLayout);

    buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
    buttonLayout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, q, [this]
    {
      progress->addError("Canceled!");
      progress->stop(true);
      finish=true;
    });
  }
};

//##################################################################################################
BlockingOperationDialog::BlockingOperationDialog(const std::function<bool()>& poll,
                                                 const QString& windowTitle,
                                                 QWidget* parent):
  QDialog(parent),
  d(new Private(poll, windowTitle))
{
  d->createUI(this, windowTitle);
}

//##################################################################################################
BlockingOperationDialog::BlockingOperationDialog(tp_utils::Progress* progress,
                                                 const QString& windowTitle,
                                                 QWidget* parent):
  QDialog(parent),
  d(new Private(progress, windowTitle))
{
  d->createUI(this, windowTitle);
}

//##################################################################################################
BlockingOperationDialog::~BlockingOperationDialog()
{
  TPSettings::setValue("BlockingOperationDialog_keepOpen", d->keepOpen->isChecked()?"true":"false");
  delete d;
}

//##################################################################################################
void BlockingOperationDialog::addWidgetLeftOfButtons(QWidget* widget)
{
  d->buttonLayout->insertWidget(d->buttonLayout->count()-1, widget);
}

//##################################################################################################
void BlockingOperationDialog::disableKeepOpen()
{
  d->keepOpen->hide();
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
                                   const std::function<bool(BlockingOperationDialog* parent, tp_utils::Progress* progress)>& closure)
{
  QPointer<BlockingOperationDialog> dialog = new BlockingOperationDialog(poll, windowTitle, parent);
  TP_CLEANUP([&]{delete dialog;});
  return exec(dialog, closure);
}

//##################################################################################################
bool BlockingOperationDialog::exec(tp_utils::Progress* progress,
                                   const QString& windowTitle,
                                   QWidget* parent,
                                   const std::function<bool(BlockingOperationDialog* parent, tp_utils::Progress*)>& closure)
{
  QPointer<BlockingOperationDialog> dialog = new BlockingOperationDialog(progress, windowTitle, parent);
  TP_CLEANUP([&]{delete dialog;});
  return exec(dialog, closure);
}

//##################################################################################################
bool BlockingOperationDialog::exec(const QPointer<BlockingOperationDialog>& dialog,
                                   const std::function<bool(BlockingOperationDialog* parent, tp_utils::Progress*)>& closure)
{
  dialog->resize(1024, 768);

  auto timer = new QTimer(dialog);
  timer->setSingleShot(true);
  timer->start(0);
  connect(timer, &QTimer::timeout, dialog, [&, dialog]
  {
    auto showCloseButton = [&]
    {
      dialog->d->allowClose = true;
      dialog->d->buttons->setStandardButtons(QDialogButtonBox::Close);
      dialog->d->buttons->disconnect(dialog->d->buttons);
      connect(dialog->d->buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
      connect(dialog->d->buttons, &QDialogButtonBox::rejected, dialog, &QDialog::accept);
    };

    bool ok = closure(dialog, dialog->d->progress);

    if(!dialog)
      return;

    dialog->d->ok = ok;

    if(dialog->d->ok)
    {
      if(dialog)
      {
        dialog->d->progress->setProgress(1.0f);
        QPalette p = dialog->d->messages->palette();
        p.setColor(QPalette::Base, QColor(201, 255, 216));
        dialog->d->messages->setPalette(p);
        if(!dialog->d->shouldKeepOpen())
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
      text->setPlainText(QString::fromStdString(dialog->d->progress->compileErrors()));
      l->addWidget(text);

      auto buttons = new QDialogButtonBox(QDialogButtonBox::Close);
      l->addWidget(buttons);

      connect(buttons, &QDialogButtonBox::accepted, errorDialog, &QDialog::accept);
      connect(buttons, &QDialogButtonBox::rejected, errorDialog, &QDialog::reject);

      auto errorTimer = new QTimer(errorDialog);
      errorTimer->setSingleShot(true);
      errorTimer->start(0);
      connect(errorTimer, &QTimer::timeout, dialog, [errorDialog, dialog, ok, showCloseButton]
      {
        errorDialog->exec();

        if(dialog)
        {
          if(ok && !dialog->d->shouldKeepOpen())
            dialog->reject();
          else
            showCloseButton();
        }
      });
    }
  });

  if(dialog->QDialog::exec() != QDialog::Accepted)
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
