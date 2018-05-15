#ifndef SVM_H
#define SVM_H

#include <QWidget>
#include <QPlainTextEdit>

class Svm : public QWidget
{
    Q_OBJECT
public:
    Svm(QWidget *parent = nullptr);
    ~Svm();

public slots:

private:
   QWidget* menu;
   QWidget* central;
   QPlainTextEdit* log;

};

#endif // SVM_H
