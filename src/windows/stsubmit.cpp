
/*
 *      TizonDife
 *      Multiple Inputs
 */

#include "stsubmit.h"
#include "src/windows/data_viz/datawidget.h"
#include <QStackedWidget>

STSubmit::STSubmit(QStackedWidget *stW, QString url): QDialog(stW)
{
    //request usgs network
    this->parent = stW;
    unet = new UNetwork(this);
    unet->setRequestFromStation(true);
    data_url = url;
    this->setWindowTitle("FDS-URGeo, Station Mgr");
    this->m_form = new QFormLayout(this);
    m_form->addRow(new QLabel("Multiple Input Test"));

    QString strs[] = {"NETWORK ID", "STATION ID", "WAVE LOC", "CHANNEL ID", "START DATE", "END DATE"};

    nId = new QLineEdit(this);
    sId = new QLineEdit(this);
    wLoc = new QLineEdit(this);
    chId = new QLineEdit(this);
    sDate = new QLineEdit(this);
    eDate = new QLineEdit(this);

    m_form->addRow(strs[0], nId);
    m_form->addRow(strs[1], sId);
    m_form->addRow(strs[2], wLoc);
    m_form->addRow(strs[3], chId);
    m_form->addRow(strs[4], sDate);
    m_form->addRow(strs[5], eDate);

    QDialogButtonBox* btn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    m_form->addRow(btn);
    QObject::connect(btn, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(btn, SIGNAL(rejected()), this, SLOT(reject()));
}

void STSubmit::accept(){
    /*
      "networkId": "IU",
      "stationId": "ANMO",
      "waveLoc": "00",
      "channelId": "BHZ",
      "startDate": "2006-09-11T00:00:00",
      "endDate": "2006-09-11T00:01:00"
      */

    //https://earthquake.usgs.gov/fdsnws/event/1/query?format=geojson&starttime=2014-01-01&endtime=2014-01-02
    /*QString url = "http://localhost:8080/urgeosismo/urgeosismo/network/wave/all";
    QString fileName = nId->text().trimmed() + "_" + sId->text().trimmed() + "_" + sDate->text().trimmed() + "_" + eDate->text() + ".data";

    unet->initDirectory("data/stations_data", fileName);
    QJsonDocument *jdoc = new QJsonDocument;
    QJsonObject obj;

    obj.insert("networkId", QJsonValue::fromVariant(nId->text()));
    obj.insert("stationId", QJsonValue::fromVariant(sId->text()));
    obj.insert("waveLoc", QJsonValue::fromVariant(wLoc->text()));
    obj.insert("channelId", QJsonValue::fromVariant(chId->text()));
    obj.insert("startDate", QJsonValue::fromVariant(sDate->text()));
    obj.insert("endDate", QJsonValue::fromVariant(eDate->text()));
    jdoc->setObject(obj);

    unet->initRequest(url, jdoc);
    */

    /*
     * NET: SB
     * STN: GVA05
    */

    if(nId->text().length() < 2){
        QMessageBox::warning(this, "FDS-URGEO PRO", "Please specify network id");
    }
    if(sId->text().length() < 2){
        QMessageBox::warning(this, "FDS-URGEO PRO", "Please specify station id");
    }
    if(sDate->text().length() < 2){
        QMessageBox::warning(this, "FDS-URGEO PRO", "Please specify the date start");
    }
    else if(eDate->text().length() < 2){
        QMessageBox::warning(this, "FDS-URGEO PRO", "Please specify the end start");
    }else{
        //QString cmd_exec = "curl -X GET 'http://192.168.0.100:8000/fdsnws/dataselect/1/query?start=2010-02-27T06:30:00.000&end=2020-02-27T10:30:00.000&nodata=404' -o temp.mseed";
        QString downlod = "curl -X GET '" + data_url + "/fdsnws/dataselect/1/query?" + "net=" + nId->text().trimmed() + "&sta=" + sId->text().trimmed() + "&loc=" + wLoc->text().trimmed() + "&cha=" + chId->text().trimmed() + "&start=" + sDate->text().trimmed() + "&end=" + eDate->text().trimmed() + "&nodata=404' -o temp.mseed";
        system(downlod.toStdString().c_str());
        std::string m2ascii = "mseed2ascii temp.mseed -o temp.data";
        system(m2ascii.c_str());

        system("mv temp.data data/stations_data/");


        //parent->addWidget(evt);
        //add data viz
        std::vector<std::string> stations;
        stations.push_back("station.data");
        stations.push_back("st2data.data");
        stations.push_back("st2data.data");
        DataWidget* waves = new DataWidget(this, stations);
        parent->insertWidget(2, waves);
        parent->setCurrentWidget(parent->widget(2));
    }
}
