#ifndef tp_qt_widgets_ExclusiveCollapsibleStack_h
#define tp_qt_widgets_ExclusiveCollapsibleStack_h

#include "tp_qt_widgets/Globals.h"

#include <QWidget>

namespace tp_qt_widgets
{

enum class StackButtonStyle
{
  RadioButton,
  Button
};

//##################################################################################################
class TP_QT_WIDGETS_SHARED_EXPORT ExclusiveCollapsibleStack: public QWidget
{
  Q_OBJECT
  TP_DQ;
public:
  //################################################################################################
  ExclusiveCollapsibleStack(StackButtonStyle buttonStyle, QWidget* parent=nullptr);

  //################################################################################################
  ~ExclusiveCollapsibleStack() override;

  //################################################################################################
  size_t addPage(const QString& title, QLayout* layout, bool expand=false);

  //################################################################################################
  size_t insertPage(size_t index, const QString& title, QLayout* layout, bool expand=false);

  //################################################################################################
  void removePage(size_t index);

  //################################################################################################
  size_t count() const;

  //################################################################################################
  QLayout* page(size_t index) const;

  //################################################################################################
  void expandPage(size_t index);

  //################################################################################################
  size_t currentPage() const;

  //################################################################################################
  void addWidgetToButton(size_t index, QWidget* widget);

  //################################################################################################
  Q_SIGNAL void currentPageChanged();

protected:
  //################################################################################################
  void resizeEvent(QResizeEvent* event) override;

  //################################################################################################
   void showEvent(QShowEvent* event) override;
};

}

#endif
