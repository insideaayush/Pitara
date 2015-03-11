#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    char* hostname=argv[1];
    char* port=argv[2];
    MainWindow w;
    w.globIp=hostname;
    w.globPort=port;
    w.show();
    return a.exec();
}
