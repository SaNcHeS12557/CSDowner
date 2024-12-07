#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QLayout>
#include <QProcess>
// #include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include <windows.h>
#include <powrprof.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timerValue(0)
    , isTimerActive(false)
    , lastDisplayedMinute(-1)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    this->setWindowTitle("CSDowner");

    // curr time
    ui->labelCurrentTime->setDigitCount(8);
    ui->labelCurrentTime->setFrameStyle(QFrame::NoFrame);

    // timer
    ui->labelTimerValue->setDigitCount(8);
    ui->labelTimerValue->setFrameStyle(QFrame::NoFrame);
    ui->labelTimerValue->setSegmentStyle(QLCDNumber::Filled);
    ui->labelTimerValue->display("00:00:00");

    // QHBoxLayout *layout = new QHBoxLayout(ui->timeOptionsWidget);
    // ui->timeOptionsWidget->setLayout(layout);
    QVBoxLayout *layout = new QVBoxLayout(ui->timeOptionsWidget);
    ui->timeOptionsWidget->setLayout(layout);

    // buttons
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearTimer);
    connect(ui->sleepModeCheckBox, &QCheckBox::toggled, this, &MainWindow::checkHibernateAvailability);

    for (QPushButton *button : ui->intervalOptionsWidget->findChildren<QPushButton*>()) {
        connect(button, &QPushButton::clicked, this, &MainWindow::handleIntervalButtonClicked);
    }

    generateTimeOptions();

    currentTimeTimer = new QTimer(this);
    connect(currentTimeTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTime);
    currentTimeTimer->start(1000);
    updateCurrentTime();

    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::countdownTick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCurrentTime()
{
    QTime currentTime = QTime::currentTime();
    // ui->labelCurrentTime->setText(currentTime.toString("HH:mm:ss"));
    ui->labelCurrentTime->display(currentTime.toString("HH:mm:ss"));

    checkMinuteChange();
}

void MainWindow::checkMinuteChange()
{
    QTime currentTime = QTime::currentTime();
    int currentMinute = currentTime.minute();

    if (currentTime.second() == 0 && currentMinute != lastDisplayedMinute) {
        generateTimeOptions();
        lastDisplayedMinute = currentMinute;
    }
}

void MainWindow::generateTimeOptions() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->timeOptionsWidget->layout());
    if (!layout) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QTime currentTime = QTime::currentTime();
    int minutes = currentTime.minute();
    int seconds = currentTime.second();
    int roundedMinutes;

    if (minutes % 15 == 0 && seconds == 0) {
        roundedMinutes = minutes + 15;
    } else {
        roundedMinutes = ((minutes + 14) / 15) * 15;
    }

    if (roundedMinutes >= 60) {
        currentTime = currentTime.addSecs(3600);
        roundedMinutes -= 60;
    }

    currentTime.setHMS(currentTime.hour(), roundedMinutes, 0);

    QVector<QTime> timeOptions;
    timeOptions.append(currentTime);
    QTime lastTime = currentTime;

    for(int i = 0; i < 3; ++i){
        lastTime = lastTime.addSecs(15 * 60);
        if(lastTime.hour() >= 24){
            lastTime = lastTime.addSecs(-24 * 3600);
        }
        timeOptions.append(lastTime);
    }

    for(int i = 0; i < 2; ++i){
        lastTime = lastTime.addSecs(30 * 60);
        if(lastTime.hour() >= 24){
            lastTime = lastTime.addSecs(-24 * 3600);
        }
        timeOptions.append(lastTime);
    }

    if(!timeOptions.isEmpty()){
        firstOptionTime = timeOptions.first();
    }

    for(const QTime &timeOption: timeOptions){
        QPushButton *timeButton = new QPushButton(timeOption.toString("HH:mm"), ui->timeOptionsWidget);
        layout->addWidget(timeButton);
        connect(timeButton, &QPushButton::clicked, this, &MainWindow::handleTimeButtonClicked);

        timeButton->setEnabled(!isTimerActive);
    }
}

void MainWindow::handleTimeButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString timeText = button->text();
    QTime selectedTime = QTime::fromString(timeText, "HH:mm");

    if (selectedTime.isValid()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation",
                                      QString("Shutdown/Sleep PC at %1 ?").arg(selectedTime.toString("HH:mm")),
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QTime currentTime = QTime::currentTime();
            int secondsToShutdown = currentTime.secsTo(selectedTime);
            if (secondsToShutdown < 0) {
                secondsToShutdown += 24 * 3600;
            }

            timerValue = secondsToShutdown;
            ui->labelTimerValue->display(QTime(0,0).addSecs(timerValue).toString("HH:mm:ss"));

            startTimer();
        } else {
            // maybe do nothing?? :)
        }
    }
}

void MainWindow::handleIntervalButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString text = button->text();
    if (text.endsWith("s")) {
        text.chop(1);
        timerValue += text.toInt();
    } else if (text.endsWith("m")) {
        text.chop(1);
        timerValue += text.toInt() * 60;
    } else if (text.endsWith("h")) {
        text.chop(1);
        timerValue += text.toInt() * 3600;
    }

    ui->labelTimerValue->display(QTime(0, 0).addSecs(timerValue).toString("HH:mm:ss"));
}

void MainWindow::startTimer()
{
    if (timerValue > 0) {
        // reverse countdown //
        if (!countdownTimer->isActive()) {
            countdownTimer->start(1000);
        }

        ui->startButton->setEnabled(false);

        QList<QPushButton*> optionButtons = ui->timeOptionsWidget->findChildren<QPushButton*>();
        for (QPushButton* button : optionButtons) {
            button->setEnabled(false);
        }

        isTimerActive = true;
    }
}

void MainWindow::clearTimer()
{
    if (countdownTimer->isActive())
        countdownTimer->stop();

    timerValue = 0;
    ui->labelTimerValue->display("00:00:00");

    ui->startButton->setEnabled(true);

    QList<QPushButton*> optionButtons = ui->timeOptionsWidget->findChildren<QPushButton*>();
    for (QPushButton* button : optionButtons) {
        button->setEnabled(true);
    }

    isTimerActive = false;
    // QProcess::execute("shutdown /a");
}

void MainWindow::countdownTick()
{
    if (timerValue > 0) {
        timerValue--;
        ui->labelTimerValue->display(QTime(0, 0).addSecs(timerValue).toString("HH:mm:ss"));
        if (timerValue == 0) {
            // time is up – shutdown
            countdownTimer->stop();
            executeShutdown();
        }
    } else {
        // timerValue == 0
        countdownTimer->stop();
    }
}

bool MainWindow::isHibernateAvailable() {
    SYSTEM_POWER_CAPABILITIES powerCapabilities;
    ZeroMemory(&powerCapabilities, sizeof(SYSTEM_POWER_CAPABILITIES));

    if (GetPwrCapabilities(&powerCapabilities)) {
        return powerCapabilities.HiberFilePresent;
    }

    return false;
}

void MainWindow::checkHibernateAvailability(bool checked) {
    if (checked) {
        if (!isHibernateAvailable()) {
            QMessageBox::warning(this, "SleepMode is Unavailable", "You cannot use hibernation mode on this PC :(");
            ui->sleepModeCheckBox->setChecked(false);
        }
    }
}

void MainWindow::executeShutdown() {
    if (ui->sleepModeCheckBox->isChecked()) {
        // sleep mode
        if (!SetSuspendState(TRUE, FALSE, FALSE)) {
            QMessageBox::critical(this, "Err", "hibernation mode failed");
        }
    } else {
        // shutdown
        HANDLE tokenHandle;
        TOKEN_PRIVILEGES tokenPrivileges;

        // open token process
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) {
            LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tokenPrivileges.Privileges[0].Luid);

            tokenPrivileges.PrivilegeCount = 1;
            tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(tokenHandle, FALSE, &tokenPrivileges, 0, nullptr, nullptr);

            if (GetLastError() == ERROR_SUCCESS) {
                UINT flags = EWX_SHUTDOWN | EWX_FORCE;
                if (!ExitWindowsEx(flags, SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_FLAG_PLANNED)) {
                    QMessageBox::critical(this, "Error", "Shutdown failed! :(");
                }

            } else {
                QMessageBox::critical(this, "Error", "Failed to adjust process privileges :(");
            }

            CloseHandle(tokenHandle);
        } else {
            QMessageBox::critical(this, "Error", "Failed to open process token :(");
        }
    }
}
