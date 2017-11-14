#include <QColor>
#include "kernel.h"
#include "ilwisdata.h"
#include "coveragedisplay/coveragelayermodel.h"
#include "drawingcolor.h"

using namespace Ilwis;
using namespace Geodrawer;

DrawingColor::DrawingColor(CoverageLayerModel *spatialdrawer) : _spatialdrawer(spatialdrawer)
{
}

QColor DrawingColor::clrVal(double rVal) const
{
    return QColor(0,0,0);
}
