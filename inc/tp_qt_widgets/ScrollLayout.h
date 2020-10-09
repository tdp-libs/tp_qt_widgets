#ifndef tp_qt_widgets_ScrollLayout_h
#define tp_qt_widgets_ScrollLayout_h

#include "tp_qt_widgets/Globals.h"

#include <QLayout>

namespace tp_qt_widgets
{


//##################################################################################################
class TP_QT_WIDGETS_SHARED_EXPORT ScrollLayout: public QLayout
{
  QList<QLayoutItem*> items;
  QRect m_geometry;
  int m_animationSpacing;
  Qt::Alignment alignment;
Q_OBJECT
public:
  //################################################################################################
  ScrollLayout(QWidget* parent=nullptr);

  //################################################################################################
  ~ScrollLayout() final;

  //################################################################################################
  virtual void addItem(QLayoutItem* item);

  //################################################################################################
  virtual void addLayout(QLayout* layout);

  //################################################################################################
  virtual QLayoutItem* itemAt(int index)const;

  //################################################################################################
  virtual QLayoutItem* takeAt(int index);

  //################################################################################################
  virtual int count()const;

  //################################################################################################
  virtual QSize sizeHint()const;

  //################################################################################################
  virtual bool hasHeightForWidth()const;

  //################################################################################################
  virtual int heightForWidth(int width)const;

  //################################################################################################
  virtual void setGeometry(const QRect& r);

  //################################################################################################
  virtual QRect geometry()const;

  //################################################################################################
  int animationSpacing()const;

  //################################################################################################
  void setAnimationSpacing(int animationSpacing);
};
}

#endif
