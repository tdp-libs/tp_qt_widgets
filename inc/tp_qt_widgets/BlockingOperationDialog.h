#ifndef tp_qt_widgets_BlockingOperationDialog_h
#define tp_qt_widgets_BlockingOperationDialog_h

#include "tp_qt_widgets/Globals.h" // IWYU pragma: keep

#include "tp_utils/Progress.h"

#include <QDialog>

namespace tp_qt_widgets
{

//##################################################################################################
class BlockingOperationDialog : public QDialog
{
  Q_OBJECT
public:
  //################################################################################################
  BlockingOperationDialog(const std::function<bool()>& poll,
                          const QString& windowTitle,
                          QWidget* parent);

  //################################################################################################
  ~BlockingOperationDialog() override;

  //################################################################################################
  void addWidgetLeftOfButtons(QWidget* widget);

  //################################################################################################
  void disableKeepOpen();

  //################################################################################################
  static bool exec(const std::function<bool()>& poll,
                   const QString& windowTitle,
                   QWidget* parent,
                   const std::function<bool(tp_utils::Progress*)>& closure);

//  //################################################################################################
//  static bool exec(const std::function<bool()>& poll,
//                   const QString& windowTitle,
//                   QWidget* parent,
//                   const std::function<bool(QWidget* parent, tp_utils::Progress*)>& closure);

  //################################################################################################
  static bool exec(const std::function<bool()>& poll,
                   const QString& windowTitle,
                   QWidget* parent,
                   const std::function<bool(BlockingOperationDialog* parent, tp_utils::Progress*)>& closure);

protected:
  //################################################################################################
  void keyPressEvent(QKeyEvent *) override;

  //################################################################################################
  void closeEvent(QCloseEvent *) override;

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
