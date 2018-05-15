

/*
 *      TizonDife
 *      Multiple Inputs
 */


#ifndef ST_SUBMIT_H
#define ST_SUBMIT_H

#include <QDialog>
#include <QFormLayout>

#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QStackedWidget>

#include "../util/unetwork.h"

class STSubmit: public QDialog
{
    //Q_OBJECT

    public:
        STSubmit(QStackedWidget *stW = 0, QString url = "");

    //public slots:
        void accept();

    private:
        QFormLayout* m_form;
        UNetwork* unet;

        QLineEdit *nId;
        QLineEdit *sId;
        QLineEdit *wLoc;
        QLineEdit *chId;
        QLineEdit *sDate;
        QLineEdit *eDate;
        QStackedWidget* parent;

        QString data_url;
};

#endif // ST_SUBMIT_H
