#include "svm.h"
#include <QDateTime>
#include <QSplitter>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>

Svm::Svm(QWidget *parent) : QWidget(parent)
{

    this->setStyleSheet("border: 0");
    //central widget
    central = new QWidget(this);
    central->setMinimumSize(600, 500);
    central->setStyleSheet("background-color: #000");

    //menu widget
    menu = new QWidget(this);
    menu->setMaximumWidth(150);
    menu->setStyleSheet("color: white;""background-color: #333;");

    log = new QPlainTextEdit(this);
    log->setStyleSheet("color: #fff;" "background-color: #000;");
    //log->setMinimumHeight(200);
    log->setReadOnly(true);
    //QString now = " " + QDateTime::currentDateTime().toString();
    QString str = "SVM Algorithm analysis";
    log->appendHtml("<font color=green> " + str + "</font>");

    QSplitter* sp = new QSplitter(Qt::Vertical);
    sp->addWidget(central);
    sp->addWidget(log);

    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->addWidget(sp);
    hl->addWidget(menu);
    this->setLayout(hl);
}

Svm::~Svm(){

}
