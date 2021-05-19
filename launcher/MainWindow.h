#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QDialog>

class QWidget;
class QLabel;
class QTimer;
class QPushButton;
class QAction;
class QMovie;
class QMenu;
class QToolBar;

class FileDownloader;

class MainWindow : public QDialog
{
    Q_OBJECT
public:
    MainWindow(bool iSilent, bool iFirstRun, QWidget *parent = 0);
    ~MainWindow();
private slots: 
    void onLatestVersionDownloaded();
    void onUpdaterDownloaded(); 
    void onUpdaterDownloadError(std::string e); 
protected:
    void closeEvent(QCloseEvent* e) override;
private: 
    bool silent, firstRun;

    QLabel *msgLabel;

    FileDownloader *latestVersionFD, *updaterFD;

    void launchApp();
};

#endif // MAINWINDOW_H
