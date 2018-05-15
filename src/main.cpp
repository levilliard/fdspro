
/*
 --> Data Analysis: 3000 lines of code
 --> Web Service 1000 lines
 --> Database 700

    Memory: ~50MB
*/

#include "fds_sismo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
