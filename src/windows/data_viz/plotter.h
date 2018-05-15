
/*-----------------------------------------
 * @levilliard
 * 2016
 *-----------------------------------------
 */


#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QTimer>
#include <iostream>

#include "plotsettings.h"

class QToolButton;
class PlotSettings;

class Plotter : public QWidget
{
 Q_OBJECT
    public:
     Plotter(QWidget *parent = 0, std::string data_file = "");
     void setPlotSettings(const PlotSettings &settings);
     void setCurveData(int id, const QVector<QPointF> &data);
     void clearCurve(int id);
     QSize minimumSizeHint() const;
     QSize sizeHint() const;
     void setMetaData(std::string& mdata);
     QString getDataTime(int second);

    public slots:
     void zoomIn();
     void zoomOut();
     void timeOut();

    protected:
     void paintEvent(QPaintEvent *event);
     void resizeEvent(QResizeEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);
     void wheelEvent(QWheelEvent *event);
    private:
     void updateRubberBandRegion();
     void refreshPixmap();
     void drawGrid(QPainter *painter);
     void drawCurves(QPainter *painter);
     enum { Margin = 40 };
     QToolButton *zoomInButton;
     QToolButton *zoomOutButton;
     QMap<int, QVector<QPointF>> curveMap;
     QVector<PlotSettings> zoomStack;
     int curZoom;
     bool rubberBandIsShown;
     QRect rubberBandRect;
     QPixmap pixmap;
     QTimer *mTimer;

     std::string file_name;

     int TIME_INTERVAL;
     int MAX_INTERVAL;
};

#endif
