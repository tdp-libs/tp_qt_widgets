#ifndef tp_qt_widgets_ExclusiveCollapsibleStack_h
#define tp_qt_widgets_ExclusiveCollapsibleStack_h

#include "tp_qt_widgets/Globals.h"

#include <QWidget>

namespace tp_qt_widgets
{

//##################################################################################################
class TP_QT_WIDGETS_SHARED_EXPORT ExclusiveCollapsibleStack: public QWidget
{
  Q_OBJECT
public:
  //################################################################################################
  ExclusiveCollapsibleStack(QWidget* parent=nullptr);

  //################################################################################################
  ~ExclusiveCollapsibleStack() override;

  //################################################################################################
  size_t addPage(const QString& title, QLayout* layout);

  //################################################################################################
  void expandPage(size_t page);

  //################################################################################################
  Q_SIGNAL void currentPageChanged();

  //################################################################################################
  size_t currentPage() const;

//protected:
//  //################################################################################################
//  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
