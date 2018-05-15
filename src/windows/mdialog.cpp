
/*
 *      TizonDife
 *      Multiple Inputs
 */

#include "mdialog.h"
#include "../windows/eventloc.h"
#include <iostream>

MDialog::MDialog(QStackedWidget *stackW)
{
    this->parent = stackW;

    //request usgs network
    unet = new UNetwork(this);
    unet->setRequestFromStation(false);

    m_log = new QPlainTextEdit(this);

    this->m_form = new QFormLayout(this);
    m_form->addRow(new QLabel(""));
    QFrame *line1 = new QFrame(this);
    line1->setFixedHeight(4);
    line1->setStyleSheet("color: white;""background-color: #333;");
    m_form->addRow(line1);

    m_form->addRow(new QLabel("EVENTS ANALYSIS PARAMETERS"));

    netId = new QLineEdit(this);
    dateStart = new QLineEdit(this);
    dateEnd = new QLineEdit(this);

    m_form->addRow("CATALOG NAME", netId);
    m_form->addRow("DATE END", dateStart);
    m_form->addRow("DATE END", dateEnd);

    cbm = new QComboBox(this);
    //QLabel* lb = new QLabel("Select Data");

    cbm->addItem("+ Select sample Events");

    //cbm->addItem("URGS NETWORKS");
    //cbm->addItem("HAITI NETWORKS");
    //cbm->addItem("R. DOMINICANA NETWORKS");
    QObject::connect(cbm, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentIndex(int)));

    //QHBoxLayout* hb = new QHBoxLayout(this);
    //hb->addWidget(lb, Qt::AlignLeft);
    //hb->addWidget(cbm, Qt::AlignLeft);

    QPushButton* btn1 = new QPushButton("Create Sample Evts", this);
    m_form->addRow(btn1);

    QFrame *line2 = new QFrame(this);
    line2->setFixedHeight(4);
    line2->setStyleSheet("color: white;""background-color: #333;");
    m_form->addRow(new QLabel(""));
    m_form->addRow(line2);

    m_form->addRow(new QLabel("SAMPLE DATA"));
    m_form->addRow(cbm);
    m_form->addRow(m_log);

    QPushButton* btn2 = new QPushButton("Validate", this);
    m_form->addRow(btn2);

    QObject::connect(btn1, SIGNAL(clicked()), this, SLOT(validateForm()));
    QObject::connect(btn2, SIGNAL(clicked()), this, SLOT(validateSample()));
}

void MDialog::setCurrentIndex(int index){

    m_log->clear();

    if(index > 0){
        currEvent = events.at(index - 1);
        QString lbs[] = {"Name", "Path", "Events", "Created by", "Date created", "Periode"};
        QString vls[6];

        vls[0] = QString::fromStdString(events.at(index - 1).sample_name);
        vls[1] = QString::fromStdString(events.at(index - 1).sample_path);
        vls[2] = QString::number(events.at(index - 1).sample_data_size);
        vls[3] = QString::fromStdString(events.at(index - 1).created_by);
        vls[4] = QString::fromStdString(events.at(index - 1).date_created);
        vls[5] = QString::fromStdString(events.at(index - 1).periode);

        m_log->setStyleSheet("color: #fff;" "background-color: #fff;");
        //log->setMinimumHeight(200);
        m_log->setReadOnly(true);

        for(int i = 0; i < 6; ++i) {
            m_log->appendHtml("<font color=green>" + lbs[i] + ": " + vls[i] + "</font>");
        }
    }
}

void MDialog::validateForm(){
    QString net = netId->text();
    QString start = dateStart->text();
    QString dend = dateEnd->text();

    if(net.length() <= 1){
        QMessageBox::warning(this, "Form Parse Error:", "Network ID is empty ");
        return ;
    }

    if(start.length() <= 1){
        QMessageBox::warning(this, "Form Parse Error:", "Date Start is empty ");
        return ;
    }

    if(dend.length() <= 1){
        QMessageBox::warning(this, "Form Parse Error:", "Date End is empty ");
        return ;
    }

    //https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime=2014-01-01&endtime=2014-01-02
    QString url = "https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime="+ start + "&endtime=" + dend;
    QString fileName = start.trimmed() + "_" + dend.trimmed() + ".csv";
    unet->initDirectory("data/sample_events", fileName);
    QJsonDocument doc;
    unet->initRequest(url, NULL);

    //if(unet->getDataSize() <= 1){
       // QMessageBox::warning(this, "Server Response", "Please verify date format. The operation could take a few minutes");
    //}else{
        //register sample to database

        nDAO = new NetworkDAO("QPSQL");
        QSqlDatabase* db = NULL;

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
             query->prepare("insert into urgeo.sample_event(sample_name, sample_path, sample_data_size, created_by, date_created, periode) values (?, ?, ?, ?, ?, ?)");

             query->addBindValue(net);
             query->addBindValue(fileName);
             query->addBindValue(unet->getDataSize());
             query->addBindValue("levilliard");
             //QDateTime now = QDateTime::QDate::currentDate();

             //std::cout<<"date = "<<now.toString().toStdString();
             query->addBindValue("2018-04-22 10:20:00");
             query->addBindValue(start + "_" + dend);

             if(query->exec())
             {
                std::cout<<"insert works well\n";

             }else{
                 std::cout<<"Query Error on sample_event: "<<db->lastError().text().toStdString()<<"\n";
             }
        }else{
            std::cout<<"Error: "<<db->lastError().text().toStdString()<<"\n";
        }

    //}
}

void MDialog::validateSample(){
    //retrieve data

    QList<Event> events;

    QFile csvFile("data/sample_events/" + QString::fromStdString(currEvent.sample_path).trimmed());

    if(csvFile.open(QIODevice::ReadOnly)){
        //read the first line
        csvFile.readLine();

        //read all line
        while(!csvFile.atEnd()){
            QString line = csvFile.readLine();
            QStringList list = line.split(",");
            //std::cout<<"->"<<list.at(2).toStdString() <<"\n";

            Event evt;
            evt.setTime(list.at(0).toStdString());
            evt.setLatitude(list.at(1).toDouble());
            evt.setLongitude(list.at(2).toDouble());
            evt.setMag(list.at(3).toDouble());
            evt.setMagType(list.at(4).toStdString());
            evt.setNST(list.at(5).toInt());
            evt.setGap(list.at(6).toDouble());
            evt.setDMin(list.at(7).toDouble());
            evt.setRMS(list.at(8).toDouble());
            evt.setNet(list.at(9).toStdString());
            evt.setId(list.at(10).toStdString());
            evt.setUpdated(list.at(11).toStdString());
            evt.setPlace(list.at(12).toStdString());
            evt.setEventType(list.at(13).toStdString());

            events.append(evt);
        }

        //close file
        csvFile.close();

    }else{
        QMessageBox::about(this, "Sample data error !", "File not found: <" + QString::fromStdString(currEvent.sample_path) + ">");
    }

    EventLoc *evt = new EventLoc();
    evt->setEventData(events, currEvent);

    parent->addWidget(evt);
    parent->setCurrentWidget(parent->widget(4));
}

void MDialog::setEventsData(std::vector<SampleEvent> &evts){
    events = evts;

    for(SampleEvent se: events){
        cbm->addItem(QString::fromStdString(se.sample_name));
    }

    update();
}

void MDialog::setRequestData(NetworkRequest &req){
    request = req;
}

MDialog::~MDialog(){
    delete unet;
    nDAO->disConnect();
    //delete nDAO;
}
