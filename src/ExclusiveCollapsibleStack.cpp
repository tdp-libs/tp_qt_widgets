#include "tp_qt_widgets/ExclusiveCollapsibleStack.h"
#include "tp_qt_widgets/CollapsiblePanel.h"

#include <QRadioButton>
#include <QBoxLayout>

namespace tp_qt_widgets
{

namespace
{
//##################################################################################################
struct Page_lt
{
  QRadioButton* button{nullptr};
  CollapsiblePanel* panel{nullptr};
};
}

//##################################################################################################
struct ExclusiveCollapsibleStack::Private
{
  QVBoxLayout* l{nullptr};
  std::vector<Page_lt> pages;

  //################################################################################################
  void updatePanels()
  {
    for(const auto& page : pages)
    {
      if(page.button->isChecked())
        page.panel->expand();
      else
        page.panel->collapse();
    }
  }
};

//##################################################################################################
ExclusiveCollapsibleStack::ExclusiveCollapsibleStack(QWidget* parent):
  QWidget(parent),
  d(new Private())
{
  d->l = new QVBoxLayout(this);
  d->l->addStretch();
}

//##################################################################################################
ExclusiveCollapsibleStack::~ExclusiveCollapsibleStack()
{
  delete d;
}

//##################################################################################################
void ExclusiveCollapsibleStack::addPage(const QString& title, QLayout* layout)
{
  auto& page = d->pages.emplace_back();

  page.button = new QRadioButton(title);
  d->l->insertWidget(d->l->count()-1, page.button);
  if(d->pages.size() == 1)
    page.button->setChecked(true);
  connect(page.button, &QRadioButton::clicked, this, [&]
  {
    d->updatePanels();
    emit currentPageChanged();
  });

  page.panel = new CollapsiblePanel();
  d->l->insertWidget(d->l->count()-1, page.panel);
  page.panel->setContentLayout(layout);

  d->updatePanels();
}

//##################################################################################################
void ExclusiveCollapsibleStack::expandPage(size_t page)
{
  if(page>=d->pages.size())
    return;

  if(const auto& button = d->pages.at(page).button; !button->isChecked())
    button->click();
}

//##################################################################################################
size_t ExclusiveCollapsibleStack::currentPage() const
{
  for(size_t p=0; p<d->pages.size(); p++)
    if(d->pages.at(p).button->isChecked())
      return p;

  return 0;
}

}
