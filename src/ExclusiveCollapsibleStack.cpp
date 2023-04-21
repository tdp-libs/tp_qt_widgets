#include "tp_qt_widgets/ExclusiveCollapsibleStack.h"
#include "tp_qt_widgets/CollapsiblePanel.h"

#include <QRadioButton>
#include <QFrame>
#include <QBoxLayout>
#include <QEvent>

namespace tp_qt_widgets
{

namespace
{
//##################################################################################################
struct Page_lt
{
  QFrame* buttonContainer{nullptr};
  QHBoxLayout* buttonLayout{nullptr};
  QRadioButton* button{nullptr};
  CollapsiblePanel* panel{nullptr};
};
}

//##################################################################################################
struct ExclusiveCollapsibleStack::Private
{
  StackButtonStyle buttonStyle;

  QVBoxLayout* l{nullptr};
  std::vector<Page_lt> pages;

  //################################################################################################
  Private(StackButtonStyle buttonStyle_):
    buttonStyle(buttonStyle_)
  {

  }

  //################################################################################################
  void updatePanels()
  {
    for(const auto& page : pages)
    {
      if(page.button->isChecked())
      {
        page.panel->expand();
        if(buttonStyle == StackButtonStyle::Button)
          page.button->setStyleSheet("QRadioButton{font-weight: bold;}");
      }
      else
      {
        page.panel->collapse();
        if(buttonStyle == StackButtonStyle::Button)
          page.button->setStyleSheet("QRadioButton{font-weight: normal;}");
      }
    }

    updateStretch();
  }

  //################################################################################################
  void updateStretch()
  {
    for(size_t i=0; i<pages.size(); i++)
      l->setStretch(int(i*2)+1, int(100.0f * pages.at(i).panel->expansionFactor()));
  }
};

//##################################################################################################
ExclusiveCollapsibleStack::ExclusiveCollapsibleStack(StackButtonStyle buttonStyle, QWidget* parent):
  QWidget(parent),
  d(new Private(buttonStyle))
{
  d->l = new QVBoxLayout(this);

  if(buttonStyle == StackButtonStyle::Button)
  {
    d->l->setSpacing(0);
  }

  d->l->setContentsMargins(0,0,0,0);
}

//##################################################################################################
ExclusiveCollapsibleStack::~ExclusiveCollapsibleStack()
{
  delete d;
}

//##################################################################################################
size_t ExclusiveCollapsibleStack::addPage(const QString& title, QLayout* layout, bool expand)
{
  return insertPage(d->pages.size(), title, layout, expand);
}

//##################################################################################################
size_t ExclusiveCollapsibleStack::insertPage(size_t index, const QString& title, QLayout* layout, bool expand)
{
  int idx = int(index)*2;

  auto& page = *d->pages.emplace(d->pages.begin()+index);

  page.buttonContainer = new QFrame();
  d->l->insertWidget(idx, page.buttonContainer, 0);

  page.buttonLayout = new QHBoxLayout(page.buttonContainer);
  page.buttonLayout->setContentsMargins(4,0,4,0);
  page.buttonLayout->setSpacing(0);

  page.button = new QRadioButton(title);
  page.buttonLayout->addWidget(page.button);

  if(d->buttonStyle == StackButtonStyle::Button)
  {
    page.buttonContainer->setFrameStyle(QFrame::Panel | QFrame::Raised);
  }

  auto setChecked = [this, button=page.button]
  {
    for(const auto& p : d->pages)
      p.button->setChecked(p.button == button);

    d->updatePanels();
    emit currentPageChanged();
  };

  connect(page.button, &QRadioButton::clicked, this, setChecked);

  page.panel = new CollapsiblePanel();
  d->l->insertWidget(idx+1, page.panel);
  page.panel->setContentLayout(layout);

  if(expand || d->pages.size() == 1)
    expandPage(index);
  else
    d->updatePanels();

  connect(page.panel, &tp_qt_widgets::CollapsiblePanel::expansionFactorChanged, this, [&]
  {
    d->updateStretch();
  });

  d->updateStretch();
  return index;
}

//##################################################################################################
void ExclusiveCollapsibleStack::removePage(size_t index)
{
  size_t selectedIndex = currentPage();

  auto page = tpTakeAt(d->pages, index);
  delete page.buttonContainer;
  delete page.panel;

  if(selectedIndex == d->pages.size())
    expandPage(selectedIndex-1);
  else if(index==selectedIndex)
    expandPage(index);
  else if(selectedIndex>index)
    expandPage(selectedIndex-1);
}

//##################################################################################################
size_t ExclusiveCollapsibleStack::count() const
{
  return d->pages.size();
}

//##################################################################################################
QLayout* ExclusiveCollapsibleStack::page(size_t index) const
{
  return d->pages.at(index).panel->contentLayout();
}

//##################################################################################################
void ExclusiveCollapsibleStack::expandPage(size_t index)
{
  blockSignals(true);
  TP_CLEANUP([&]{blockSignals(false);});

  if(index>=d->pages.size())
    return;

  if(const auto& button = d->pages.at(index).button; !button->isChecked())
  {
    for(size_t i=0; i<d->pages.size(); i++)
      d->pages.at(i).button->setChecked(i==index);

    d->updatePanels();
  }
}

//##################################################################################################
size_t ExclusiveCollapsibleStack::currentPage() const
{
  for(size_t p=0; p<d->pages.size(); p++)
    if(d->pages.at(p).button->isChecked())
      return p;

  return 0;
}

//##################################################################################################
void ExclusiveCollapsibleStack::addWidgetToButton(size_t index, QWidget* widget)
{
  d->pages.at(index).buttonLayout->addWidget(widget);
}

//##################################################################################################
void ExclusiveCollapsibleStack::resizeEvent(QResizeEvent* event)
{
  for(const auto& page : d->pages)
    page.panel->recalculateGeometry();

  d->updateStretch();
  QWidget::resizeEvent(event);
}

//##################################################################################################
void ExclusiveCollapsibleStack::showEvent(QShowEvent* event)
{
  d->updateStretch();
  QWidget::showEvent(event);
}

}
