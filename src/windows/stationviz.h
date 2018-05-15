
/*
 * URGEO Seismic Data Analysis
 * October 2016
 * levilliard@gmail.com
 * -----------------------------------------
 *
 */

#ifndef EVENTVIZ_H
#define EVENTVIZ_H

#include <QObject>
#include <QWidget>

#include "./map/MMap.h"
#include "../model/station.h";
#include "../model/network.h"

#include <QPushButton>
#include <vector>
#include <QPlainTextEdit>
#include <QComboBox>


#include <QSurfaceFormat>
#include "ArcGISRuntimeEnvironment.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QMessageBox>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValueIterator>

#include <fstream>
#include <iostream>

class QByteArray;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;

class StationViz: public QWidget
{
    Q_OBJECT

    public:
        StationViz(QWidget* parent=nullptr);
        ~StationViz();

    public slots:
        void resetCamera();
        void selectNetwork(int code);

    private:
        MMap* map;
        QWidget* menu;
        std::vector<Station> slist;
        std::vector<Network> nlist;
        QPushButton*  reset;
        QPlainTextEdit* log;
        QComboBox* network_codes;

        void initPosition();
        void initNetworkList();
};

#endif // EVENTVIZ_H
