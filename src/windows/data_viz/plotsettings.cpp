/*-----------------------------------------
 * @levilliard
 * FDS SISMO
 * 2016
 *-----------------------------------------
 */

#include "plotsettings.h"

PlotSettings::PlotSettings()
{
    /*
     * [minX, maxX]
     */
    minX = 0.0;
    maxX = 1000.0;

    /*
     * [minY, maxY]
     */
    minY = 0.0;
    maxY = 1000.0;

    /*
     * Scale from x, y
     */
     numXTicks = 20;
     numYTicks = 20;
}

//----------------------------------------------------------------
void PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;
    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

//---------------------------------------------------------------
void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

//-------------------------------------------------------------------
void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }

    numTicks = int(std::ceil(max / step) - std::floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;

    min = std::floor(min / step) * step;
    max = std::ceil(max / step) * step;
}
