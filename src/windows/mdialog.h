

/*
 *      TizonDife
 *      Multiple Inputs
 */


#ifndef MDIALOG_H
#define MDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QJsonDocument>

#include "../model/event.h"
#include "../model/network.h"
#include "../util/unetwork.h"
#include "src/dao/networkdao.h"

#include <vector>

class MDialog: public QDialog
{
    Q_OBJECT

    public:
        MDialog(QStackedWidget* stackW);
        ~MDialog();

    public slots:
        void validateSample();
        void validateForm();
        void setCurrentIndex(int index);
        void setEventsData(std::vector<SampleEvent>& evts);
        void setRequestData(NetworkRequest& req);

    private:
        QFormLayout* m_form;
        QStackedWidget* parent;
        QComboBox* cbm;
        std::vector<SampleEvent> events;
        SampleEvent currEvent;
        NetworkRequest request;
        QPlainTextEdit *m_log;

        UNetwork *unet;

        QLineEdit* netId;
        QLineEdit* dateStart;
        QLineEdit* dateEnd;

        NetworkDAO* nDAO = new NetworkDAO("QPSQL");
};

#endif // MDIALOG_H
