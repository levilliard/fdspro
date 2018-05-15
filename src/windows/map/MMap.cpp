

#include "MMap.h"

#include <QMessageBox>
#include "QVBoxLayout"
#include <iostream>

using namespace Esri::ArcGISRuntime;

MMap::MMap(QWidget *parent /*=nullptr*/):
    QWidget(parent),
    m_map(nullptr),
    m_mapView(nullptr)
{

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);
    m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

    // Create a map using the nationalGeographic BaseMap
    m_map = new Map(Basemap::nationalGeographic(this), this);
    graphicsOverlay = new GraphicsOverlay(this);
    //buoySymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 10);

    // Set map to map view
    m_mapView->setMap(m_map);
    m_mapView->setMinimumSize(600, 500);

    m_mapView->graphicsOverlays()->append(graphicsOverlay);

    QVBoxLayout* vl = new QVBoxLayout();
    vl->addWidget(m_mapView);
    setLayout(vl);
}

// destructor
MMap::~MMap()
{

}

void MMap::populateBuoyLocations(const QList<Point> locations, QColor color){

  buoySymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::green), 15);

  for (int i = 0; i < 2; ++i){
      graphicsOverlay->graphics()->append(new Graphic(locations[i], buoySymbol, this));
  }

  buoySymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 15);

  for (int i = 2; i < locations.size(); ++i){
      graphicsOverlay->graphics()->append(new Graphic(locations[i], buoySymbol, this));
  }

  m_mapView->update();
}


