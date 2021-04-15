#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QDialog>
#include <QSystemTrayIcon>

class QWidget;
class QLabel;
class QTimer;
class QPushButton;
class QAction;
class QMovie;
class QSystemTrayIcon;
class QMenu;
class QToolBar;

<<<<<<< HEAD
class FileDownloader;
=======
//class FileDownloader;
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5

class XMRStak;

class MainWindow : public QDialog
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots: 
    void onQuitButtonClicked();
    void onInfoButtonClicked();
    void onPauseButtonClicked();
    void onResumeButtonClicked();
    void onSettingsButtonClicked();
    void onUpdateStats();
<<<<<<< HEAD
    void onLatestVersionDownloaded();
    void onUpdaterDownloaded(); 
=======
    void onBackgroundImageDownloaded();
    void onBusyIndicatorImageDownloaded();
    void onTrayIconImageDownloaded();
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onIconMessageClicked();
protected:
    void closeEvent(QCloseEvent* e) override;
private: 
    XMRStak* xmrstak = nullptr;

<<<<<<< HEAD
=======
    QToolBar* toolBar;
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5
    QMenu* menu;
    QTimer *updateTimer;
    QLabel* backgroundImageLabel, *statsLabel;
    QAction *pauseAction, *resumeAction; 
    QSystemTrayIcon* systemTrayIcon;
<<<<<<< HEAD
    QWidget  *toolBarWidget;

    FileDownloader *latestVersionFD, *updaterFD;
=======

//    FileDownloader* backgroundFD, * busyIndicatorFD, * trayIconFD;
>>>>>>> 51ea25f571f17f36eb4fb357672fc1b40b3a23e5

    QAction* addAction(QAction* a);
    QAction* addAction(const QString& action);
    void updateButtonState();
};

#endif // MAINWINDOW_H
