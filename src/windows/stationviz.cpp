
#include "stationviz.h"

using namespace Esri::ArcGISRuntime;

StationViz::StationViz(QWidget *parent): QWidget(parent)
{
    //create the map
    std::cout<<"\n StationViz init ...\n";
    map = new MMap(this);
    map->setMinimumSize(600, 500);

    //Network request
    //unet = new UNetwork();

    //Create log
    log = new QPlainTextEdit(this);
    log->setStyleSheet("color: #fff;" "background-color: #000;");
    //log->setMinimumHeight(200);
    log->setReadOnly(true);

    QDateTime now = QDateTime::currentDateTime();
    log->appendHtml("<font color=green>" + now.toString() + "</font>"+ ":Sismo network, " + QString::number(slist.size()) + " stations");


    //init map (with station's position)
    initPosition();

    menu = new QWidget();
    menu->setMaximumWidth(150);
    menu->setStyleSheet("color: white;""background-color: #333;");

    //network list
    network_codes = new QComboBox(menu);
    QObject::connect(network_codes,SIGNAL(currentIndexChanged(int)),this,SLOT(selectNetwork(int)));

    //init network list
    initNetworkList();
    QLabel* lb = new QLabel("Choose");

    QHBoxLayout* hb = new QHBoxLayout(this);
    hb->addWidget(lb, Qt::AlignLeft);
    hb->addWidget(network_codes, Qt::AlignLeft);

    /*
     *
    reset = new QPushButton("Reset", this);
    QObject::connect(reset, SIGNAL(clicked()), this, SLOT(resetCamera()));

    QVBoxLayout *vl = new QVBoxLayout();
    vl->addLayout(hb);
    vl->addWidget(reset);

    */

    menu->setLayout(hb);

    QSplitter* sp = new QSplitter(Qt::Vertical);
    sp->addWidget(map);
    sp->addWidget(log);

    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->addWidget(sp);
    hl->addWidget(menu);
    this->setLayout(hl);
}

StationViz::~StationViz(){

}

void StationViz::resetCamera(){
    //this->map->resetCamera();
    update();
}

void StationViz::initPosition(){

    //QString url = "https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime=2014-01-01&endtime=2014-01-02";
    //QString data = "{\"networkId\": \"IU\", \"startDate\": \"1993-01-01\", \"endDate\": \"2002-01-01d\"}";
    //this->unet->initDirectory("script", "iu.data");
    //this->unet->initRequest(url);

    slist.push_back(Station("001", "ST1", 19.87741, -72.95046, "null", "Port-de-Paix, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("002", "ST2", 19.66379, -72.03448, "null", "Cap-Haitien, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("003", "ST3", 18.95349, -72.066, "null", "Boukan Kare, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("004", "ST4", 18.46784, -72.02717, "null", "Ganthier, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("005", "ST5", 18.44578, -72.54824, "null", "Leogane, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("006", "ST6", 18.82245, -73.06819, "null", "Lagonave, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("007", "ST7", 18.48384, -73.35392, "null", "Petit-Trou de Nippe, Haiti", "no rm", "2017-02-11"));
    slist.push_back(Station("008", "ST8", 18.53573, -74.08985, "null", "Jeremie, Fermate, Haiti", "no rm", "2017-02-11"));

    //add points(stations position), all positions will
    QList<Point> locations;
    SpatialReference sr(4326);

    for(auto stn: slist){
        locations.push_back(Point(stn.getStationLon(), stn.getStationLat(), sr));
    }

    map->populateBuoyLocations(locations, Qt::red);

    for(int i = 0; i < slist.size(); ++i){
        QString str = "<font color=gray>" + QString::fromStdString(slist[i].getStationDesc()) + ", Long: " + QString::number(slist[i].getStationLon()) + " Lat: " + QString::number(slist[i].getStationLat()) + "</font>" ;
        log->appendHtml(str);
    }
}

void StationViz::initNetworkList(){

    nlist.push_back(Network("", "URGEO", "URGEO(Unité de Recherche en Géotechnique)", "Haiti", "", ""));
    nlist.push_back(Network("", "AY", "Bureau of Mines and Energy (Haiti)", "Haiti", "", ""));
    nlist.push_back(Network("", "AV", "USGS Alaska Fairbanks", "Haiti", "United States", ""));
    nlist.push_back(Network("", "UI", "University of Idaho", "Haiti", "United States", ""));

    for(auto net: nlist){
        network_codes->addItem(QString::fromStdString(net.getNetworkCode()));
    }

}

void StationViz::selectNetwork(int code){
    QString str = "<font color=green> Network Selected Code " + QString::fromStdString(nlist[code].getNetworkCode()) + ", " + QString::fromStdString(nlist[code].getNetworkInstitution()) + "</font>" ;
    log->appendHtml(str);
}

