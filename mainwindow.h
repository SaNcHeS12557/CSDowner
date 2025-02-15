#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCurrentTime();
    void generateTimeOptions(); // time list generation
    void handleTimeButtonClicked();
    void handleIntervalButtonClicked();
    void startTimer();
    void clearTimer();

    void executeShutdown();
    void countdownTick();
    void checkMinuteChange();
    bool isHibernateAvailable();
    void checkHibernateAvailability(bool checked); // sleep availability


protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QTimer *currentTimeTimer;
    QTimer *optionsUpdateTimer;
    int timerValue;
    QTime firstOptionTime;
    QTimer *countdownTimer;
    bool isTimerActive;

    int lastDisplayedMinute;
};

#endif // MAINWINDOW_H
