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
  TP_DQ;
public:
  //################################################################################################
  FileDialogLineEdit(QWidget* parent = nullptr);

  //################################################################################################
  virtual ~FileDialogLineEdit();

  //################################################################################################
  void setText(const QString& text);

  //################################################################################################
  QString text() const;

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
  /*!
  The file extension filter in the format "*.png *.jpg"
  */
  void setFilter(const QString& filter);

  //################################################################################################
  void setQSettingsPath(const std::string& qSettingsPath);

Q_SIGNALS:
  //################################################################################################
  void selectionChanged();
};

}

#endif
