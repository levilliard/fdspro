
#include "fds_sismo.h"
#include "windows/stsubmit.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow){
    //init UI
    ui->setupUi(this);
    this->setMinimumWidth(1000);
    this->setWindowTitle("FDS SISMO");
    ui->stationEdit->setText("http://192.168.0.100:8000");

    msg = "FDS PRO APP MSG";

    //menu action
    QObject::connect(ui->actionlevilliard, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->actionNetwork, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->actionNetwork, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->actionStations, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->actionAbout_URGEO, SIGNAL(triggered()), this, SLOT(showDefaultMsg()));
    QObject::connect(ui->stationGoBtn, SIGNAL(clicked()), this, SLOT(goOtherStation()));

    //remove margin central widget
    ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);
    isDataViz = false;
     /*
        //add hover mouse on item in side menu
        //ui->station_icon->setStyleSheet("QLabel:hover {background: #444;}");
        //ui->event_icon->setStyleSheet("QLabel:hover {background: #444;}");
        //i->wave_icon->setStyleSheet("QLabel:hover {background: #444;}");
        ui->svm_icon->setStyleSheet("QLabel:hover {background: #444;}");
     */

    //add data viz
    std::vector<std::string> stations;
    stations.push_back("station.data");
    stations.push_back("st2data.data");
    //stations.push_back("st2data.data");
    DataWidget* waves = new DataWidget(this, stations);

    //add stations mgr

    StationViz *station_viz = new StationViz(this);

    //[2] Station
    ui->stackedWidget->addWidget(waves);
    ui->stackedWidget->addWidget(station_viz);

    ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(3));

    //QObject::connect(ui->svmBtn, SIGNAL(clicked()), this, SLOT(goSVM()));
    QVBoxLayout* vl = new QVBoxLayout();
    MDialog *svm_form = new MDialog(ui->stackedWidget);
    vl->addWidget(svm_form);
    ui->bmenu->setLayout(vl);

    //Database
    //http://tufangorel.blogspot.com/2015/07/connect-to-postgresql-from-qt-application-with-qtsql.html

    NetworkDAO* nDAO = new NetworkDAO("QPSQL");
    QSqlDatabase* db = NULL;
    std::vector<Network> network_list;
    std::vector<SampleEvent> events;
    std::map<std::string, std::vector<Station>> net_map;

    try{
        db = nDAO->connect("localhost", "URGEO_DB", "postgres", "supernova");
        std::cout <<"DB OPENED !\n";
    }catch(std::exception &e){
        std::cout <<" TEST NOT OPENED !\n";
        //qFatal("Error %s sending event to object", e.what());
    }

    if(db->open()){
         QSqlQuery* query = new QSqlQuery(*db);
         std::cout <<"init sql query ... !\n";

         query->setForwardOnly(true);

         // Select empty person table
         Network old_ntw;
         old_ntw.setNetworkId("-1");

         if(query->exec(QString("SELECT a.*, b.* from urgeo.network a left join urgeo.station b on a.network_id = b.network_id")))
         {
            std::vector<Station> stations;

            while(query->next()){
                Network ntw;
                Station st;

                ntw.setNetworkId(query->value(0).toString().toStdString());
                ntw.setNetworkCode(query->value(2).toString().toStdString());
                ntw.setNwetworkInstitution(query->value(3).toString().toStdString());
                ntw.setNetworkRemarks(query->value(4).toString().toStdString());

                st.setStationId(query->value(9).toString().toStdString());
                st.setStationCode(query->value(11).toString().toStdString());
                st.setStationLat(query->value(12).toString().toDouble());
                st.setStationLon(query->value(13).toString().toDouble());
                st.setStationDesc(query->value(15).toString().toStdString());

                stations.push_back(st);

                if(old_ntw.getNeworkId() != ntw.getNeworkId()){
                    network_list.push_back(ntw);
                    old_ntw = ntw;
                    net_map[old_ntw.getNetworkCode()] = stations;
                    stations.clear();
                }
            }
         }
         else{
             std::cout<<"Query Error on network, station: "<<db->lastError().text().toStdString()<<"\n";
         }

         if(query->exec(QString("SELECT * from urgeo.sample_event ")))
         {
             while(query->next()){
                 SampleEvent evt;
                 evt.sample_id = query->value(0).toInt();
                 evt.sample_name = query->value(1).toString().toStdString();
                 evt.sample_path = query->value(2).toString().toStdString();
                 evt.sample_data_size = query->value(3).toInt();
                 evt.created_by = query->value(4).toString().toStdString();
                 evt.date_created = query->value(5).toString().toStdString();
                 evt.periode = query->value(8).toString().toStdString();

                 events.push_back(evt);
             }

             svm_form->setEventsData(events);
             svm_form->update();

         }else{
             std::cout<<"Query Error on sample_event: "<<db->lastError().text().toStdString()<<"\n";
         }

    }else{
        std::cout<<"Error: "<<db->lastError().text().toStdString()<<"\n";
    }

    //QTreeWidget
    ui->network_menu->setColumnCount(1);
    QList<QTreeWidgetItem *> items;

    //add networks
    for(auto &n: net_map){
        QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::fromStdString(n.first)));
        items.append(item);
        QObject::connect(ui->network_menu, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(selectItem(QTreeWidgetItem*, int)));

        //add stations
        for(unsigned int i = 0; i < n.second.size(); i++){
            QString temp(QString::fromStdString(n.second[i].getStationCode() + ":" + n.second[i].getStationDesc()));
            QTreeWidgetItem* sub_item = new QTreeWidgetItem((QTreeWidget*)0, QStringList(temp));
            items.at(i)->addChild(sub_item);
        }
    }

    ui->network_menu->insertTopLevelItems(0, items);
    nDAO->disConnect();
}

void MainWindow::goOtherStation(){
    QString url = ui->stationEdit->text();

    if(url.length() < 3){
        QMessageBox::warning(this, "FDS PRO", "Please specify the url of the data center");
    }else{
        STSubmit *ss = new STSubmit(ui->stackedWidget, url);
        ss->show();
    }
}

void MainWindow::selectItem(QTreeWidgetItem* item, int index){
    isDataViz = ! isDataViz;

    if(isDataViz){
        ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(3));
    }else{
        ui->stackedWidget->setCurrentWidget(ui->stackedWidget->widget(2));
    }
}


void MainWindow::showDefaultMsg(){
    QMessageBox::warning(this, "FDS PRO", msg);
}

MainWindow::~MainWindow(){
    delete ui;
}
