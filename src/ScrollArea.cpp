#include "tdp_qt_widgets/ScrollArea.h"

#include <QLayout>
#include <QEvent>
#include <QPainter>
#include <QDebug>
#include <QLinearGradient>
#include <QScrollBar>

namespace tdp_qt_widgets
{
class ScrollArea::Private: public QWidget
{
public:
  ScrollArea* q;
  bool fadeEnabled;


  //################################################################################################
  Private(ScrollArea* q_):
    QWidget(q_),
    q(q_),
    fadeEnabled(true)
  {
    setAttribute( Qt::WA_TransparentForMouseEvents );
    setPalette(Qt::transparent);
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute( Qt::WA_OpaquePaintEvent, true );
    setAutoFillBackground(false);
    setStyleSheet("QWidget{background-color: transparent;}");
    setAttribute(Qt::WA_NoSystemBackground);
  }

  //################################################################################################
  void paintEvent(QPaintEvent* event) final
  {
    TP_UNUSED(event);
    if(fadeEnabled)
    {
      QPainter painter(this);
      painter.setPen(Qt::NoPen);

      int vPos=100;
      int vLen=100;
      if(q->verticalScrollBar())
      {
        vPos = q->verticalScrollBar()->value();
        vLen = q->verticalScrollBar()->maximum();
      }

      QLinearGradient gradient;
      gradient.setColorAt(0.0f, QColor(0, 0, 0, 70));
      gradient.setColorAt(1.0f, QColor(0, 0, 0, 0));

      {
        QRect fadeRect(rect().topLeft(), QSize(rect().width(), tpBound(0, vPos/24, 12)));
        gradient.setStart(0, 0);
        gradient.setFinalStop(0, fadeRect.height());
        painter.setBrush(gradient);
        painter.drawRect(fadeRect);
      }
      {
        int fadeHeight = tpMin((vLen-vPos)/24, 12);
        int fadeTop = rect().height()-fadeHeight;
        QRect fadeRect(rect().x(), fadeTop, rect().width(), fadeHeight);
        gradient.setStart(0, rect().height());
        gradient.setFinalStop(0, fadeTop);
        painter.setBrush(gradient);
        painter.drawRect(fadeRect);
      }
    }
  }

  //################################################################################################
  void calculate()
  {
    if(q->widget())
      q->widget()->setGeometry(0, 0, q->viewport()->width(), q->widget()->layout()->heightForWidth(q->viewport()->width()));

    setGeometry(q->viewport()->geometry());
  }
};

//##################################################################################################
ScrollArea::ScrollArea(QWidget* parent):
  QScrollArea(parent),
  d(new Private(this))
{
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameShape(QFrame::NoFrame);
  d->calculate();
}

//##################################################################################################
void ScrollArea::resizeEvent(QResizeEvent* event)
{
  d->calculate();
  QScrollArea::resizeEvent(event);
}

//##################################################################################################
void ScrollArea::showEvent(QShowEvent* event)
{
  d->calculate();
  QScrollArea::showEvent(event);
}

//##################################################################################################
bool ScrollArea::viewportEvent(QEvent* event)
{
  if(event->type() == QEvent::LayoutRequest)
  {
    d->calculate();
  }

  return QAbstractScrollArea::viewportEvent(event);
}

//##################################################################################################
void ScrollArea::enableFade(bool fade)
{
  d->fadeEnabled = fade;
  update();
}

//##################################################################################################
bool ScrollArea::isFadeEnabled()
{
  return d->fadeEnabled;
}

}
