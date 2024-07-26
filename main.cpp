#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QSettings>


bool isFromStartup()
{
    QStringList args = QCoreApplication::arguments();
    return args.contains("--startup");
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (!isFromStartup()) {
        w.show();
    } else {
        qDebug() << "程序是从自启动启动的，不显示主窗口。";
    }
    return a.exec();
}
