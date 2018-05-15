#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

#include "src/windows/stationviz.h"
#include "src/windows/data_viz/datawidget.h"
#include "src/dao/networkdao.h"
#include "src/windows/mdialog.h"
#include "src/model/network.h"

#include <iostream>
#include <vector>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void selectItem(QTreeWidgetItem*item, int index);
    void showDefaultMsg();
    void goOtherStation();

private:
    Ui::MainWindow *ui;
    bool isDataViz;
    QString msg;
};

#endif // MAINWINDOW_H
