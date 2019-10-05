#include "tp_qt_widgets/PainterUtils.h"

#include <QFont>
#include <QStringList>

namespace tp_qt_widgets
{

//##################################################################################################
Qt::BrushStyle brushStyleFromString(const QString& brushStyleString)
{
  if(brushStyleString == "NoBrush")               return Qt::NoBrush;
  if(brushStyleString == "SolidPattern")          return Qt::SolidPattern;
  if(brushStyleString == "Dense1Pattern")         return Qt::Dense1Pattern;
  if(brushStyleString == "Dense2Pattern")         return Qt::Dense2Pattern;
  if(brushStyleString == "Dense3Pattern")         return Qt::Dense3Pattern;
  if(brushStyleString == "Dense4Pattern")         return Qt::Dense4Pattern;
  if(brushStyleString == "Dense5Pattern")         return Qt::Dense5Pattern;
  if(brushStyleString == "Dense6Pattern")         return Qt::Dense6Pattern;
  if(brushStyleString == "Dense7Pattern")         return Qt::Dense7Pattern;
  if(brushStyleString == "HorPattern")            return Qt::HorPattern;
  if(brushStyleString == "VerPattern")            return Qt::VerPattern;
  if(brushStyleString == "CrossPattern")          return Qt::CrossPattern;
  if(brushStyleString == "BDiagPattern")          return Qt::BDiagPattern;
  if(brushStyleString == "FDiagPattern")          return Qt::FDiagPattern;
  if(brushStyleString == "DiagCrossPattern")      return Qt::DiagCrossPattern;
  if(brushStyleString == "LinearGradientPattern") return Qt::LinearGradientPattern;
  if(brushStyleString == "RadialGradientPattern") return Qt::RadialGradientPattern;
  if(brushStyleString == "ConicalGradientPattern")return Qt::ConicalGradientPattern;
  if(brushStyleString == "TexturePattern")        return Qt::TexturePattern;

  return Qt::SolidPattern;
}

//##################################################################################################
Qt::PenStyle penStyleFromString(const QString& penStyleString)
{
  if(penStyleString == "NoPen")          return Qt::NoPen;
  if(penStyleString == "SolidLine")      return Qt::SolidLine;
  if(penStyleString == "DashLine")       return Qt::DashLine;
  if(penStyleString == "DotLine")        return Qt::DotLine;
  if(penStyleString == "DashDotLine")    return Qt::DashDotLine;
  if(penStyleString == "DashDotDotLine") return Qt::DashDotDotLine;
  if(penStyleString == "CustomDashLine") return Qt::CustomDashLine;

  return Qt::SolidLine;
}

//##################################################################################################
Qt::PenCapStyle penCapStyleFromString(const QString& penCapStyleString)
{
  if(penCapStyleString == "FlatCap")      return Qt::FlatCap;
  if(penCapStyleString == "SquareCap")    return Qt::SquareCap;
  if(penCapStyleString == "RoundCap")     return Qt::RoundCap;
  if(penCapStyleString == "MPenCapStyle") return Qt::MPenCapStyle;

  return Qt::FlatCap;
}

//##################################################################################################
Qt::PenJoinStyle penJointStyleFromString(const QString& penJointStyleString)
{
  if(penJointStyleString == "MiterJoin")     return Qt::MiterJoin;
  if(penJointStyleString == "BevelJoin")     return Qt::BevelJoin;
  if(penJointStyleString == "RoundJoin")     return Qt::RoundJoin;
  if(penJointStyleString == "SvgMiterJoin")  return Qt::SvgMiterJoin;
  if(penJointStyleString == "MPenJoinStyle") return Qt::MPenJoinStyle;

  return Qt::MiterJoin;
}

//##################################################################################################
Qt::Alignment alignmentFromString(const QString& penJointStyleString)
{
  QList<Qt::AlignmentFlag> flags;

  QStringList parts = penJointStyleString.split('|');

  Q_FOREACH(QString part, parts)
  {
    part = part.simplified();

    if     (part == "AlignLeft")            flags.append(Qt::AlignLeft);
    else if(part == "AlignLeading")         flags.append(Qt::AlignLeading);
    else if(part == "AlignRight")           flags.append(Qt::AlignRight);
    else if(part == "AlignTrailing")        flags.append(Qt::AlignTrailing);
    else if(part == "AlignHCenter")         flags.append(Qt::AlignHCenter);
    else if(part == "AlignJustify")         flags.append(Qt::AlignJustify);
    else if(part == "AlignAbsolute")        flags.append(Qt::AlignAbsolute);
    else if(part == "AlignHorizontal_Mask") flags.append(Qt::AlignHorizontal_Mask);
    else if(part == "AlignTop")             flags.append(Qt::AlignTop);
    else if(part == "AlignBottom")          flags.append(Qt::AlignBottom);
    else if(part == "AlignVCenter")         flags.append(Qt::AlignVCenter);
    else if(part == "AlignBaseline")        flags.append(Qt::AlignBaseline);
    else if(part == "AlignVertical_Mask")   flags.append(Qt::AlignVertical_Mask);
    else if(part == "AlignCenter")          flags.append(Qt::AlignCenter);
  }

  Qt::Alignment alignment=nullptr;
  Q_FOREACH(Qt::AlignmentFlag flag, flags)
    alignment |= flag;

  return alignment;
}

}
