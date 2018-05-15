/*
 * ------------------------------------------
 * @levilliard
 * 2016
 * ------------------------------------------
 */


#ifndef PLOTSETTINGS_H
#define PLOTSETTINGS_H

#include <cmath>

class PlotSettings
{
    public:
        PlotSettings();
        void scroll(int dx, int dy);
        void adjust();
        double spanX() const { return maxX - minX; }
        double spanY() const { return maxY - minY; }
        double minX;
        double maxX;
        int numXTicks;
        double minY;
        double maxY;
        int numYTicks;

    private:
        static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif // PLOTSETTINGS_H
