#include "eventloc.h"
#include <bits/stdc++.h>

using namespace Esri::ArcGISRuntime;

EventLoc::EventLoc(QWidget *parent) : QWidget(parent), m_map(nullptr), m_mapView(nullptr)
{
    this->setStyleSheet("border: 0");
    //central widget

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);
    m_mapView->setWrapAroundMode(WrapAroundMode::Disabled);

    // Create a map using the nationalGeographic BaseMap
    m_map = new Map(Basemap::nationalGeographic(this), this);

    // Set map to map view
    m_mapView->setMap(m_map);
    m_mapView->setMinimumSize(600, 500);

    SimpleMarkerSymbol* buoySymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 10);
    GraphicsOverlay* graphicsOverlay = new GraphicsOverlay(this);
    QList<Point> buoyLocations;

    populateBuoyLocations(buoyLocations);

    for (const auto& location: buoyLocations)
    {
        graphicsOverlay->graphics()->append(new Graphic(location, buoySymbol, this));
    }

    m_mapView->graphicsOverlays()->append(graphicsOverlay);

    //menu widget
    menu = new QWidget(this);
    menu->setMaximumWidth(200);
    //menu->setStyleSheet("color: white;""background-color: #999;");
    createMenu();

    log = new QPlainTextEdit(this);
    log->setStyleSheet("color: #fff;" "background-color: #000;");
    //log->setMinimumHeight(200);
    log->setReadOnly(true);
    //QString now = " " + QDateTime::currentDateTime().toString();
    QString str = "Events location";
    log->appendHtml("<font color=green> " + str + "</font>");

    QSplitter* sp = new QSplitter(Qt::Vertical);
    sp->addWidget(m_mapView);
    sp->addWidget(log);

    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->addWidget(sp);
    hl->addWidget(menu);
    this->setLayout(hl);
}

void EventLoc::createMap(){

}

void EventLoc::createMenu(){
    this->m_form = new QFormLayout(menu);
    m_form->addRow(new QLabel("SVM ANALYSIS "));
    m_form->addRow(new QLabel(" "));
    m_form->addRow(new QLabel("Area: "));

    QSignalMapper *mapper = new QSignalMapper();

    //add form param
    minLat = new QLineEdit(this);
    //minLat->setStyleSheet("color: #fff;" "background-color: #999;");

    maxLat = new QLineEdit(this);
   // maxLat->setStyleSheet("color: #fff;" "background-color: #999;");

    maxLon = new QLineEdit(this);
    //maxLon->setStyleSheet("color: #fff;" "background-color: #999;");

    minLon = new QLineEdit(this);
    //minLon->setStyleSheet("color: #fff;" "background-color: #999;");

    m_form->addRow("Max Lat", maxLat);
    m_form->addRow("Min Lat", minLat);
    m_form->addRow("Max Lon", maxLon);
    m_form->addRow("Min Lon", minLon);

    //lEdit->setStyleSheet("color: #fff;" "background-color: #555;");

    m_form->addRow(new QLabel(" "));

    //summary
    m_form->addRow(new QLabel("SUMMARY: "));
    QPushButton* sum = new QPushButton("View", this);
    sum->setFixedWidth(40);
    sum->setStyleSheet("color: #fff;" "background-color: #555;");
    m_form->addRow(sum);
    m_form->addRow(new QLabel(" "));

    //svm
    m_form->addRow(new QLabel("SVM: "));
    QPushButton* svm = new QPushButton("View", this);
    svm->setFixedWidth(40);
    svm->setStyleSheet("color: #fff;" "background-color: #555;");
    m_form->addRow(svm);
    m_form->addRow(new QLabel(" "));

    //regression
    m_form->addRow(new QLabel("MUl R. R."));
    QPushButton* reg = new QPushButton("View", this);
    reg->setFixedWidth(40);
    reg->setStyleSheet("color: #fff;" "background-color: #555;");
    m_form->addRow(reg);
    m_form->addRow(new QLabel(" "));

    //QObject::connect(sum, SIGNAL(clicked()), this, SLOT(callSumary()));
    QObject::connect(sum, SIGNAL(released()), mapper, SLOT(map()));
    mapper->setMapping(sum, 1); // Number to be passed in the slot

    QObject::connect(svm, SIGNAL(released()), mapper, SLOT(map()));
    mapper->setMapping(svm, 2); // Number to be passed in the slot

    QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(calSVMStep(int)));
}

void EventLoc::setEventData(QList<Event>& evts, SampleEvent &spEvent){

    log->appendPlainText("Name: " + QString::fromStdString(spEvent.sample_name));
    log->appendPlainText("Path: " + QString::fromStdString(spEvent.sample_path));
    log->appendPlainText("Events: " + QString::number(evts.size()));
    log->appendPlainText("Periode: " + QString::fromStdString(spEvent.periode));
    log->appendPlainText("Created by: " + QString::fromStdString(spEvent.created_by));
    log->appendPlainText("Date created: " + QString::fromStdString(spEvent.date_created));

    this->events = evts;
    sampleEvent = spEvent;

    //update
    SimpleMarkerSymbol* buoySymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 10);
    GraphicsOverlay* graphicsOverlay = new GraphicsOverlay(this);
    QList<Point> buoyLocations;

    populateBuoyLocations(buoyLocations);

    for (const auto& location: buoyLocations)
    {
        graphicsOverlay->graphics()->append(new Graphic(location, buoySymbol, this));
    }

    m_mapView->graphicsOverlays()->append(graphicsOverlay);

   this->update();
}

void EventLoc::populateBuoyLocations(QList<Point>& locations)
{
  SpatialReference sr(4326);
  locations.clear();

  for (auto& evt: events){
      locations.append(Point(evt.getLatitude(), evt.getLongitude(), sr));
  }
}

void EventLoc::update(){

}

void EventLoc::calSVMStep(int step){

    if(sampleEvent.sample_name.length() <= 1){
        QMessageBox::warning(this, "Data Sample Error:", "We can't find data sample in your directoty ");
        return ;
    }

    if(maxLat->text().length() <= 1){
        QMessageBox::warning(this, "Data Sample Parameters Error:", "Max Latitude doesn'tspecify corectly ! ");
        return;
    }

    if(minLat->text().length() <= 1){
        QMessageBox::warning(this, "Data Sample Parameters Error:", "Min Latitude doesn'tspecify corectly ! ");
        return;
    }

    if(maxLon->text().length() <= 1){
        QMessageBox::warning(this, "Data Sample Parameters Error:", "Max Longitude doesn'tspecify corectly ! ");
        return;
    }

    if(minLon->text().length() <= 1){
        QMessageBox::warning(this, "Data Sample Parameters Error:", "Min Logitude doesn'tspecify corectly ! ");
        return;
    }

    //check number order here


    //remove file extension, get file name
    size_t lastindex = sampleEvent.sample_path.find_last_of(".");
    std::string file_name = sampleEvent.sample_path.substr(0, lastindex);

    //svm summary
    if(step == 1){
        std::string temp = "Rscript scripts/summary.R data/sample_events/" + file_name + ".csv " + maxLat->text().toStdString() + " " + minLat->text().toStdString() + " " + maxLon->text().toStdString() + " " + minLon->text().toStdString() + " " + file_name + "_SUMM.png > summary.out";
        system(temp.c_str());

        QString title = QString::fromStdString(file_name);
        showLogResult("summary.out");
        viewResult(title);
    }else if(step == 2){
        std::string temp = "Rscript scripts/svm.R data/sample_events/" + file_name + ".csv " + maxLat->text().toStdString() + " " + minLat->text().toStdString() + " " + maxLon->text().toStdString() + " " + minLon->text().toStdString() + " " + file_name + "_SVM.png > svm.out";
        system(temp.c_str());

        //QString title = QString::fromStdString(file_name);
        showLogResult("svm.out");
        //viewResult(title);
    }else if(step == 3){

    }
}

void EventLoc::viewResult(const QString fileName){

   ImgDialog *dl = new ImgDialog(this, fileName);
    dl->exec();
}

EventLoc::~EventLoc(){

}

void EventLoc::showLogResult(const QString fileName){
    log->clear();
    log->appendHtml("<font color=green>  Events Classification </font>");

    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly)){
        std::cout<<"file openned !\n";
        //read all line
        while(!file.atEnd()){
            QString line = file.readLine();
            log->appendPlainText(line);
        }

        //close file
        file.close();
    }else{
        std::cout<<"log file not openned !\n";
    }
}
