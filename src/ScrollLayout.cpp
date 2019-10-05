#include "tp_qt_widgets/ScrollLayout.h"

namespace tp_qt_widgets
{
//##################################################################################################
ScrollLayout::ScrollLayout(QWidget* parent):
  QLayout(parent),
  m_animationSpacing(0),
  alignment(Qt::AlignCenter)
{
  setContentsMargins(0, 0, 0, 0);
}

//##################################################################################################
ScrollLayout::~ScrollLayout()
{
  QLayoutItem *child = takeAt(0);
  while(child)
  {
    delete child;
    child = takeAt(0);
  }
}

//##################################################################################################
void ScrollLayout::addItem(QLayoutItem* item)
{
  items.append(item);
  invalidate();
  update();
}

void ScrollLayout::addLayout(QLayout* layout)
{
  if (!adoptLayout(layout))
    return;

  addItem(layout);
}

//##################################################################################################
QLayoutItem* ScrollLayout::itemAt(int index)const
{
  return (index>=0 && index<items.size())?items.at(index):nullptr;
}

//##################################################################################################
QLayoutItem* ScrollLayout::takeAt(int index)
{
  return (index>=0 && index<items.size())?items.takeAt(index):nullptr;
}

//##################################################################################################
int ScrollLayout::count()const
{
  return items.size();
}

//##################################################################################################
QSize ScrollLayout::sizeHint()const
{
  int height = 0;
  int width = geometry().width();

  int iMax = items.size();
  for(int i=0; i<iMax; i++)
  {
    QLayoutItem* item = items.at(i);

    int h = item->heightForWidth(width);

    if(h<=0)
      h = item->sizeHint().height();

    if(h>0)
      height+=h+spacing();
  }

  return QSize(width, height);
}

//##################################################################################################
bool ScrollLayout::hasHeightForWidth()const
{
  return true;
}

//##################################################################################################
int ScrollLayout::heightForWidth(int width)const
{
  int height = 0;

  int iMax = items.size();
  for(int i=0; i<iMax; i++)
  {
    QLayoutItem* item = items.at(i);

    int h = item->heightForWidth(width);

    if(h<=0)
      h = item->sizeHint().height();

    if(h>0)
      height+=h+spacing();
  }

  return height;
}

//##################################################################################################
void ScrollLayout::setGeometry(const QRect& r)
{
  m_geometry = r;
  int y = r.y();
  int w = r.width();
  int x = r.x();

  int iMax = items.size();
  for(int i=0; i<iMax; i++)
  {
    QLayoutItem* item = items.at(i);

    int h = item->heightForWidth(w);

    if(h<=0)
      h = item->sizeHint().height();

    switch(alignment)
    {
    case Qt::AlignCenter:
    {
      int aw = tpMin(item->maximumSize().width(), w);
      item->setGeometry(QRect(x+((w-aw)/2), y, aw, h));
      break;
    }

    default:
      item->setGeometry(QRect(x, y, w, h));
      break;
    }
    y+=h+spacing();

    y+=m_animationSpacing;
  }
}

//##################################################################################################
QRect ScrollLayout::geometry()const
{
  return m_geometry;
}

//################################################################################################
int ScrollLayout::animationSpacing()const
{
  return m_animationSpacing;
}

//################################################################################################
void ScrollLayout::setAnimationSpacing(int animationSpacing)
{
  m_animationSpacing = animationSpacing;
  setGeometry(m_geometry);
}
}
