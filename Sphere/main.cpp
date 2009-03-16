#include <QtGui/QApplication>
#include <QDebug>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    
//    qDebug()<<QCoreApplication::instance();
//    qDebug("sph === %d",QCoreApplication::instance());
    return a.exec();
}
