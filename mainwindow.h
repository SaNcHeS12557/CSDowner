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
    void updateCurrentTime();  // update cur time
    void generateTimeOptions(); // time list generation
    void handleTimeButtonClicked(); // button click processing
    void handleIntervalButtonClicked(); // timer interval
    void startTimer(); // timer start
    void clearTimer(); // clear timer
    void executeShutdown();
    void countdownTick();
    void checkMinuteChange();
    bool isHibernateAvailable();
    void checkHibernateAvailability(bool checked);


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
