// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef MAP_H
#define MAP_H

#include <QWidget>

#include <Camera.h>
#include "SimpleMarkerSymbol.h"
#include "MapGraphicsView.h"
#include "Map.h"
#include "Basemap.h"

namespace Esri
{
    namespace ArcGISRuntime
    {
        class Map;
        class MapGraphicsView;
    }
}

class MMap : public QWidget
{
    Q_OBJECT
public:
    MMap(QWidget *parent = nullptr);
    ~MMap();
    void populateBuoyLocations(const QList<Esri::ArcGISRuntime::Point> locations, QColor color);

public slots:

private:
    Esri::ArcGISRuntime::Map* m_map;
    Esri::ArcGISRuntime::MapGraphicsView* m_mapView;
    Esri::ArcGISRuntime::SimpleMarkerSymbol* buoySymbol;
    Esri::ArcGISRuntime::GraphicsOverlay* graphicsOverlay;
};

#endif // MAP_H
