#include "mainwindow.h"
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QTextCodec>
#include<QSplashScreen>
#include<QDateTime>
#include<QtGui>
int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
     QTextCodec::setCodecForLocale(codec);


     QPixmap pixmap(":/Resource/splash.PNG");
     QSplashScreen splash(pixmap);
     for(qint64 i=8666666;i>0;i--)
         splash.show();



     MainWindow w;
     w.setWindowIcon(QIcon(":/Resource/tongji.ico"));
     QFont font("Microsoft YaHei", 9, 50, false);
     w.setFont(font);

     splash.finish(&w);
     w.show();

     return a.exec();
}
