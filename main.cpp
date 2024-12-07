#include "mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QPushButton>

#include <QApplication>
#include <QDebug>
#include <Windows.h>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
