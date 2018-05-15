#include "networkdao.h"

NetworkDAO::NetworkDAO(const char *driver)
{
    db = new QSqlDatabase(QSqlDatabase::addDatabase(driver));
}

NetworkDAO::~NetworkDAO()
{
    qDebug()<< "Called Destructor!";

    if(db){
       delete db;
    }
}

QSqlDatabase* NetworkDAO::connect(
        const QString &server,
        const QString &databaseName,
        const QString &userName,
        const QString &password)
{
    db->setConnectOptions();
    db->setHostName(server);
    db->setDatabaseName(databaseName);
    db->setUserName(userName);
    db->setPassword(password);

    if(db->open()) {
        return db;
    }
    else {
        return NULL;
    }
}



int NetworkDAO::selectRowCountResult(QSqlQuery* query)
{
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query->lastError().text();
        return -1;
    }

    int recordCount = 0;
    while (query->next())
    {
        qDebug() << "Field 1 : " << query->value(0).toString()
                 << "Field 2 : " << query->value(1).toString();
        ++recordCount;
    }

    return recordCount;
}

bool NetworkDAO::executeInsert(QSqlQuery* query)
{
    db->transaction();
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query->lastError().text();
        db->rollback();
        return false;
    }
    db->commit();
    return true;
}

bool NetworkDAO::executeUpdate(QSqlQuery* query)
{
    db->transaction();
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query->lastError().text();
        db->rollback();
        return false;
    }
    db->commit();
    return true;
}

bool NetworkDAO::executeDelete(QSqlQuery* query)
{
    db->transaction();
    bool queryRes = query->exec();
    if (query->lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query->lastError().text();
        db->rollback();
        return false;
    }
    db->commit();
    return true;
}

void NetworkDAO::disConnect()
{
    qDebug() << "Disconnected From Database!";
    if(db != NULL){
        db->close();
        delete db;
    }
}

