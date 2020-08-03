#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class QLabel;
class QTimer;
class QPushButton;
class QAction;
class QMovie;

class XMRStak;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots: 
    void onInfoButtonClicked();
    void onPauseButtonClicked();
    void onResumeButtonClicked();
    void onUpdateStats();
private:
    bool paused = true;
    XMRStak* xmrstak = nullptr;

    QTimer *updateTimer;
    QWidget *mainWidget;
    QLabel  *statsLabel, *busyIndicatorLabel;
    QAction *pauseAction, *resumeAction, *quitAction, *infoAction;
    QMovie* busyIndicatorMovie;

    void updateButtonState();
};

#endif // MAINWINDOW_H
