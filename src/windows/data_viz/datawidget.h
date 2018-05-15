#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "plotter.h"

#include <QWidget>
#include <QPlainTextEdit>
#include <iostream>

#include <string>

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget *parent = 0, std::vector<std::string> stations = {"station.data"});
    ~DataWidget();


signals:

public slots:

private:
    QWidget* menu;
    Plotter* plotter;
    QPlainTextEdit* log;    //void createMenu();

};

#endif // DATAWIDGET_H
