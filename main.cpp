#include "mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QPushButton>

#include <QApplication>
#include <QDebug>
#include <Windows.h>
#include <QPalette>

void printPalette(const QPalette &palette) {
    qDebug() << "Palette:";
    qDebug() << "WindowText:" << palette.color(QPalette::WindowText).name();
    qDebug() << "Button:" << palette.color(QPalette::Button).name();
    qDebug() << "Light:" << palette.color(QPalette::Light).name();
    qDebug() << "Midlight:" << palette.color(QPalette::Midlight).name();
    qDebug() << "Dark:" << palette.color(QPalette::Dark).name();
    qDebug() << "Mid:" << palette.color(QPalette::Mid).name();
    qDebug() << "Text:" << palette.color(QPalette::Text).name();
    qDebug() << "BrightText:" << palette.color(QPalette::BrightText).name();
    qDebug() << "ButtonText:" << palette.color(QPalette::ButtonText).name();
    qDebug() << "Base:" << palette.color(QPalette::Base).name();
    qDebug() << "Window:" << palette.color(QPalette::Window).name();
    qDebug() << "Shadow:" << palette.color(QPalette::Shadow).name();
    qDebug() << "Highlight:" << palette.color(QPalette::Highlight).name();
    qDebug() << "HighlightedText:" << palette.color(QPalette::HighlightedText).name();
    qDebug() << "Link:" << palette.color(QPalette::Link).name();
    qDebug() << "LinkVisited:" << palette.color(QPalette::LinkVisited).name();
    qDebug() << "AlternateBase:" << palette.color(QPalette::AlternateBase).name();
    qDebug() << "ToolTipBase:" << palette.color(QPalette::ToolTipBase).name();
    qDebug() << "ToolTipText:" << palette.color(QPalette::ToolTipText).name();
    qDebug() << "PlaceholderText:" << palette.color(QPalette::PlaceholderText).name();
    qDebug() << "Accent:" << palette.color(QPalette::Accent).name();
}

void setCustomPalette(QApplication &app) {
    QPalette customPalette;

    customPalette.setColor(QPalette::WindowText, QColor("#ffffff"));
    customPalette.setColor(QPalette::Button, QColor("#3c3c3c"));
    customPalette.setColor(QPalette::Light, QColor("#787878"));
    customPalette.setColor(QPalette::Midlight, QColor("#5a5a5a"));
    customPalette.setColor(QPalette::Dark, QColor("#1e1e1e"));
    customPalette.setColor(QPalette::Mid, QColor("#282828"));
    customPalette.setColor(QPalette::Text, QColor("#ffffff"));
    customPalette.setColor(QPalette::BrightText, QColor("#33a5ff"));
    customPalette.setColor(QPalette::ButtonText, QColor("#ffffff"));
    customPalette.setColor(QPalette::Base, QColor("#2d2d2d"));
    customPalette.setColor(QPalette::Window, QColor("#1e1e1e"));
    customPalette.setColor(QPalette::Shadow, QColor("#000000"));
    customPalette.setColor(QPalette::Highlight, QColor("#0063b1"));
    customPalette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    customPalette.setColor(QPalette::Link, QColor("#0063b1"));
    customPalette.setColor(QPalette::LinkVisited, QColor("#003966"));
    customPalette.setColor(QPalette::AlternateBase, QColor("#003966"));
    customPalette.setColor(QPalette::ToolTipBase, QColor("#3c3c3c"));
    customPalette.setColor(QPalette::ToolTipText, QColor("#d4d4d4"));
    customPalette.setColor(QPalette::PlaceholderText, QColor("#ffffff"));
    customPalette.setColor(QPalette::Accent, QColor("#0063b1"));

    customPalette.setColor(QPalette::Disabled, QPalette::Button, QColor("#1e1e1e"));
    customPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#787878"));

    app.setPalette(customPalette);
}

int main(int argc, char *argv[])
{
    // qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
    QApplication a(argc, argv);

    setCustomPalette(a);
    MainWindow w;
    w.show();

    return a.exec();
}
