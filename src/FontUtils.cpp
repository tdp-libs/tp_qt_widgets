#include "tp_qt_widgets/FontUtils.h"

#include <QFont>
#include <QFontMetricsF>
#include <QStringList>

namespace tp_qt_widgets
{

//##################################################################################################
QFont fontFromString(QString fontString)
{
  QFont font;

  bool found=false;
  do
  {
    found=false;

    int first = fontString.indexOf('\"');

    if(first != -1)
    {
      int next = fontString.indexOf('\"', first+1);

      if(next != -1)
      {
        font.setFamily(fontString.mid(first+1, (next-first)-1).simplified());
        fontString.remove(first, 1+(next-first));
        found = true;
      }
    }
  }
  while(found);

  QStringList parts = fontString.split(' ', QString::SkipEmptyParts);

  for(int i=0; i<parts.size(); i++)
    parts[i] = parts.at(i).toLower();

  Q_FOREACH(const QString& part, parts)
  {
    if(part == "bold")
      font.setBold(true);

    else if(part == "italic")
      font.setItalic(true);

    else if(part == "overline")
      font.setOverline(true);

    else if(part == "underline")
      font.setUnderline(true);

    else if(part == "strike-out")
      font.setStrikeOut(true);

    else if(part.endsWith("pt"))
    {
      bool ok = false;
      float pt = part.leftRef(part.size()-2).toFloat(&ok);

      if(ok)
        font.setPointSizeF(qreal(pt));
    }
    else if(part.endsWith("px"))
    {
      bool ok = false;
      int px = part.leftRef(part.size()-2).toInt(&ok);

      if(ok)
        font.setPixelSize(px);
    }
  }

  return font;
}

//##################################################################################################
QString fontToString(const QFont& font)
{
  QString fontString = QString("\"%1\" ").arg(font.family());

  if(font.bold())
    fontString.append("bold ");

  if(font.italic())
    fontString.append("italic ");

  if(font.overline())
    fontString.append("overline ");

  if(font.underline())
    fontString.append("underline ");

  if(font.strikeOut())
    fontString.append("strike-out ");

  fontString.append(QString("%1pt").arg(font.pointSizeF()));

  return fontString.simplified();
}

//##################################################################################################
int pixelSize(float em, const QFont& font)
{
  QFontMetricsF fm = QFontMetricsF(font);

  QChar m('M');
  return int(em * float(fm.width(m)));
}

}
