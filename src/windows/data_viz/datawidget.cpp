/*-----------------------------------------
 * @levilliard
 * FDS SISMO
 * 2016
 *-----------------------------------------
 */

#include "datawidget.h"
#include "stationconfig.h"

#include <QSplitter>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPointF>
#include <QVector>
#include <QGridLayout>
#include <QPushButton>
#include <string>
#include <vector>

DataWidget::DataWidget(QWidget *parent, std::vector<std::string> stations) : QWidget(parent)
{
    this->setStyleSheet("border: 0");
    plotter = new Plotter(this, "station.data");

    //[0] get seismic waves data
    StationConfig scfg;

    std::vector<float> data1 = scfg.getStationData(stations[0]);


    if(data1.size()){
        QVector<QPointF> seismic_data1;

        for(unsigned int i = 0; i < data1.size(); ++i)
            seismic_data1.push_back(QPointF(i, (float)data1[i]/3000));

        plotter->setCurveData(0, seismic_data1);

    }

    if(stations.size() >= 2){
        std::vector<float> data = scfg.getStationData(stations[1]);
        QVector<QPointF> seismic_data;

        if(data.size()){
            for(unsigned int i = 0; i < data.size(); ++i)
                seismic_data.push_back(QPointF(i, (float)data[i]/3000));
        }

        plotter->setCurveData(1, seismic_data);
    }



    if(stations.size() >= 3){
        std::vector<float> data = scfg.getStationData(stations[2]);
        QVector<QPointF> seismic_data;

        if(data.size()){
            for(unsigned int i = 0; i < data.size(); ++i)
                seismic_data.push_back(QPointF(i, (float)data[i]/3000));
        }

        plotter->setCurveData(2, seismic_data);
    }


    plotter->setMinimumSize(600, 500);
    plotter->setStyleSheet("background-color: #000");

    //menu widget
    menu = new QWidget(this);
    menu->setMaximumWidth(150);
    menu->setStyleSheet("color: white;""background-color: #333;");

    log = new QPlainTextEdit(this);
    log->setStyleSheet("color: #fff;" "background-color: #000;");
    //log->setMinimumHeight(200);
    log->setReadOnly(true);
    //QString now = " " + QDateTime::currentDateTime().toString();
    QString str = "Seimic Waves Vizualization Algorithm analysis";
    log->appendHtml("<font color=green> " + str + "</font>");
    QString str2 = QString::fromStdString(scfg.getMetaData());
    log->appendPlainText(str2);

    QSplitter* sp = new QSplitter(Qt::Vertical);
    sp->addWidget(plotter);
    sp->addWidget(log);

    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->addWidget(sp);
    hl->addWidget(menu);
    this->setLayout(hl);
}

DataWidget::~DataWidget(){

}

