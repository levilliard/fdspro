#include "imgdialog.h"
#include "ui_imgdialog.h"

ImgDialog::ImgDialog(QWidget *parent, QString fileName) :
    QDialog(parent),
    ui(new Ui::ImgDialog)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);

    //scene->setSceneRect(0, 0, 1000, 1000);

    ui->viewImg->setScene(scene);

    QPixmap pm(fileName + "_SUMM.png");

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pm.scaled(QSize(this->width(), this->height())));
    scene->addItem(item);
}

ImgDialog::~ImgDialog(){
    delete ui;
    delete scene;
}
