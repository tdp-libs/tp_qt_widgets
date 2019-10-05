#ifndef tp_qt_widgets_FileDialogLineEdit_h
#define tp_qt_widgets_FileDialogLineEdit_h

#include "tp_qt_widgets/Globals.h"

#include <QWidget>
#include <QFileDialog>

namespace tp_qt_widgets
{

//##################################################################################################
//! An animation made up from key frames
/*!

*/
class TP_QT_WIDGETS_SHARED_EXPORT FileDialogLineEdit: public QWidget
{
  Q_OBJECT
public:
  //################################################################################################
  FileDialogLineEdit(QWidget* parent = nullptr);

  //################################################################################################
  virtual ~FileDialogLineEdit();

  //################################################################################################
  void setText(const QString& text);

  //################################################################################################
  QString text()const;

  //################################################################################################
  void setInitialDirectory(const QString& dir);

  enum Mode
  {
    DirectoryMode,
    OpenFileMode,
    SaveFileMode
  };

  //################################################################################################
  void setMode(Mode mode);

  //################################################################################################
  void setFilter(const QString& filter);

signals:
  //################################################################################################
  void selectionChanged();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
