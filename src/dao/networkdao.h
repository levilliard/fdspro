#ifndef NETWORKDAO_H
#define NETWORKDAO_H

#include <QtSql>
#include <QString>
#include <QDebug>

class NetworkDAO
{
public:
    NetworkDAO(const char* driver);
    ~NetworkDAO();

    QSqlDatabase* connect( const QString& server,
                           const QString& databaseName,
                           const QString& userName,
                           const QString& password );
    void disConnect();

    int selectRowCountResult(QSqlQuery* query);
    bool executeInsert(QSqlQuery* query);
    bool executeUpdate(QSqlQuery* query);
    bool executeDelete(QSqlQuery* query);

private:
    QSqlDatabase* db;

};

#endif // NETWORKDAO_H
