#ifndef EVENTLOC_H
#define EVENTLOC_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QFormLayout>
#include <QList>
#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFile>

#include "Basemap.h"
#include "Map.h"
#include "MapGraphicsView.h"
#include "imgdialog.h"

#include <SimpleMarkerSymbol.h>
#include <QPushButton>

#include <iostream>
#include <sstream>

#include "../model/event.h";

namespace Esri
{
    namespace ArcGISRuntime
    {
        class Map;
        class MapGraphicsView;
        class Point;
        class SimpleMarkerSymbol;
        class GraphicsOverlay;
    }
}

class EventLoc : public QWidget
{
    Q_OBJECT
public:
    explicit EventLoc(QWidget *parent = 0);
    ~EventLoc();
    void createMenu();
    void populateBuoyLocations(QList<Esri::ArcGISRuntime::Point>& locations);
    void setEventData(QList<Event> &evts, SampleEvent& spEvent);
    void update();

signals:

public slots:
    void calSVMStep(int step);

private:
    QList<Event> events;
    QWidget* central;
    QPlainTextEdit* log;
    QWidget* menu;
    QFormLayout* m_form;
    SampleEvent sampleEvent;

    //add form param
    QLineEdit* minLat;
    QLineEdit* maxLat;
    QLineEdit* maxLon;
    QLineEdit* minLon;

    Esri::ArcGISRuntime::Map* m_map;
    Esri::ArcGISRuntime::MapGraphicsView* m_mapView;
    Esri::ArcGISRuntime::SimpleMarkerSymbol* buoySymbol;
    Esri::ArcGISRuntime::GraphicsOverlay* graphicsOverlay;

    void createMap();
    void viewResult(const QString fileName);
    void showLogResult(const QString fileName);

};

#endif // EVENTLOC_H
