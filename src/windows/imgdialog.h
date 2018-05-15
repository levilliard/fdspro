#ifndef IMGDIALOG_H
#define IMGDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

namespace Ui {
class ImgDialog;
}

class ImgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImgDialog(QWidget *parent = 0, QString fileName = "");
    ~ImgDialog();

private:
    Ui::ImgDialog *ui;
    QGraphicsScene *scene;
};

#endif // IMGDIALOG_H
