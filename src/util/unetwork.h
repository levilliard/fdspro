
/*
 * URGEO Seismic Data Analysis
 * October 2016
 * levilliard@gmail.com
 * -----------------------------------------
 *
 * usage:
 *  1. Create an object
 *  2. Call the method initDirectory(...)
 *  3. Call the method  initRequest(...)
 *
 */

#ifndef UNETWORK_H
#define UNETWORK_H

#include <QWidget>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QMessageBox>

#include <QList>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QPushButton>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariantMap>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

//#include "src/global/global.h"

class QByteArray;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;

class UNetwork : public QWidget
{
    Q_OBJECT

public:
    UNetwork(QWidget *parent = 0, QString dir = "data", QString fileName = "test.csv");
    ~UNetwork();

    /*
     * initialization of the http request, a post http request require a url and data parameters
     *  - url: url
     *  - parameter: data
     * The request will handle to our web service api create on top of the IRIS_WS
     *
     */
    void initRequest(QString &uri, QJsonDocument *data);
    void initDirectory(QString dir, QString file);
    int getDataSize();
    int completed();
    void setRequestFromStation(bool req);

public slots:
    /*
     * When the request is complete, we have to store data localy
     *  - we create a directory (if not exist) with the network name
     *  - we create a file (if not exist) with all the network's stations
     *  - we create a data file (if not exist) for every station
     */
    void onResult(QNetworkReply* reply);

private:
    QNetworkAccessManager * mNetMan;
    QNetworkReply * mNetReply;
    QByteArray * mDataBuffer;

    QString directory;
    QString file_name;
    int data_size;
    int completed_request;  // 0 request not fired, 1 request completed, -1 request error
    void createCSVFile(const QString& dir , const QString &fileName, QVector<QStringList>& data);
    void readCsvFile(const QString filePath, QList<QStringList>& data);

    bool requestFromStation;
};

#endif // UNETWORK_H
