#include "unetwork.h"

#include "ext/json.hpp"
#include <sstream>

//using namespace GLOBAL_FUNCTION;

UNetwork::UNetwork(QWidget * parent, QString dir, QString fileName)
    : QWidget(parent)
    , mNetMan(new QNetworkAccessManager(this))
    , mNetReply(nullptr)
    , mDataBuffer(new QByteArray){
    data_size = 0;
    directory = dir;
    file_name = fileName;
    completed_request = 0;
    requestFromStation = false;
}

void UNetwork::initRequest(QString &uri, QJsonDocument *data){

    QUrl url(uri);
    QNetworkRequest req;

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    req.setUrl(url);

    if(requestFromStation && data != NULL){
        std::cout<<" request from other stations\n";
        //std::cout<<data->toJson().toStdString()<<"\n";
        mNetReply = mNetMan->post(req, data->toJson());
    }else{
        std::cout<<" request from usgs network\n";
        mNetReply = mNetMan->get(req);
    }
    connect(mNetMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResult(QNetworkReply*)));
}

void UNetwork::setRequestFromStation(bool req){
    requestFromStation = req;
}

void UNetwork::onResult(QNetworkReply* reply)
{
    std::cout<<"Retrieve data....\n";

    if (mNetReply->error() != QNetworkReply::NoError){
        std::cout<<mNetReply->errorString().toStdString()<<"\n";
        completed_request = -1;
        return;
    }

    QString response = (QString)reply->readAll();
    std::cout<<"network reply ...\n";

    if(!requestFromStation){
        QVector<QStringList> data;

        QStringList head = {"time", "latitude", "longitude", "depth", "mag", "magType", "nst", "gap", "dmin", "rms", "net", "id", "updated", "place", "type"};
        data.push_back(head);

        nlohmann::json js = nlohmann::json::parse(response.toStdString());
        //std::cout<<js["features"][0]["properties"]<<"\n";

        data_size = js["metadata"]["count"];
        std::cout<<" data size = "<<data_size;

        if(data_size == NULL || data_size <= 0){
            completed_request = -1;
            std::cout<<"no data found !\n";
            return;
        }

        completed_request =  1;
        QString cond = "";

        int i = 0;
        while(cond != "null"){
            nlohmann::json obj = js["features"][i];
            QStringList list;
            list.append(QString::fromStdString(obj["properties"]["time"].dump()));
            list.append(QString::fromStdString(obj["geometry"]["coordinates"][1].dump()));
            list.append(QString::fromStdString(obj["geometry"]["coordinates"][0].dump()));
            list.append(QString::fromStdString(obj["geometry"]["coordinates"][2].dump()));
            list.append(QString::fromStdString(obj["properties"]["mag"].dump()));
            list.append(QString::fromStdString(obj["properties"]["magType"].dump()));
            list.append(QString::fromStdString(obj["properties"]["nst"].dump()));
            list.append(QString::fromStdString(obj["properties"]["gap"].dump()));
            list.append(QString::fromStdString(obj["properties"]["dmin"].dump()));
            list.append(QString::fromStdString(obj["properties"]["rms"].dump()));
            list.append(QString::fromStdString(obj["properties"]["net"].dump()));
            //list.append(QString::fromStdString(obj["properties"]["id"].dump()));
            list.append("id" + QString::number(i + 100));
            list.append(QString::fromStdString(obj["properties"]["updated"].dump()));
            list.append(QString::fromStdString(obj["properties"]["place"].dump()));
            list.append(QString::fromStdString(obj["properties"]["type"].dump()).remove('"'));

            if(QString::fromStdString(obj["properties"]["time"].dump()).length()>5){
                if(QString::fromStdString(obj["properties"]["mag"].dump()) != "null" && QString::fromStdString(obj["properties"]["nst"].dump()) != "null" && QString::fromStdString(obj["properties"]["gap"].dump()) != "null" && QString::fromStdString(obj["properties"]["dmin"].dump()) != "null" && QString::fromStdString(obj["properties"]["rms"].dump()) != "null"){
                  data.append(list);
                }
            }

            cond = QString::fromStdString(obj["type"].dump());
            ++i;
        }
        createCSVFile(directory, file_name, data);
    }else{
        nlohmann::json js = nlohmann::json::parse(response.toStdString())[0];
        QString ncode = QString::fromStdString(js["networkCode"].dump());
        QString scode = QString::fromStdString(js["stationCode"].dump());
        QString loc = QString::fromStdString(js["location"].dump());
        QString ccode = QString::fromStdString(js["channelCode"].dump());

        QString ssample = QString::fromStdString(js["segments"]["samplerate"].dump());
        QString stime = QString::fromStdString(js["segments"]["startTime"].dump());
        QString setime = QString::fromStdString(js["segments"]["endTime"].dump());
        QString scount = QString::fromStdString(js["segments"]["sampleCount"].dump());
        QString sdata = QString::fromStdString(js["segments"]["intData"].dump());

        QString head = ncode + " " + scode + " " + ccode + " " + loc + "ccode" + " " + ssample + " " + stime + " " + setime +  " " + scount + "\n";

        QFile file(directory + "/" + file_name);
        if (file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            stream << head<< endl;
            stream << sdata << endl;
            file.close();
        }else{
            std::cout<<"station data: file not opened\n";
        }

    }
}

void UNetwork::createCSVFile(const QString &dir , const QString &fileName, QVector<QStringList>& data){
    QStringList hd = data.at(0);

    QtCSV::StringData strData;
    strData.addRow(hd);

    for(int i = 1; i <data.length(); ++i){
        strData.addRow(data.at(i));
    }
    // write to file
    QString filePath = QDir::currentPath() + "/" + dir + "/" + fileName;
    QtCSV::Writer::write(filePath, strData);
}

void UNetwork::readCsvFile(const QString filePath, QList<QStringList>& data){
    data = QtCSV::Reader::readToList(filePath);
}

void UNetwork::initDirectory(QString dir, QString file){
    std::cout<<"file and directory:"<<dir.toStdString()<<", "<<file.toStdString()<<"\n";
    directory = dir;
    file_name = file;
}

int UNetwork::getDataSize(){
    return data_size;
}

int UNetwork::completed(){
    return completed_request;
}

UNetwork::~UNetwork(){
   if(mNetReply){
       delete mNetReply;
   }

   if(mNetMan){
       delete mNetMan;
   }
}
