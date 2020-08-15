#include "MainWindow.h"

#include <iostream>
#include <sstream>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSize>
#include <QScreen>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QToolBar>
#include <QTimer>
#include <QMovie>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFile>

#include <Windows.h>
//#include <processthreadsapi.h>

#include "InfoDialog.h"
//#include "FileDownloader.h"

#include "xmrstak.h"

//#include "cpuminer/miner.h"


void MainWindow::onInfoButtonClicked() { InfoDialog(this).exec(); }
void MainWindow::onSettingsButtonClicked() { }

void MainWindow::onQuitButtonClicked() {
    updateTimer->stop();
    systemTrayIcon->hide();
    delete systemTrayIcon;
    QCoreApplication::quit();
}

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
    std::stringstream ss; ss << "EPU Power Co-op v0.0." << GUI_VERSION << ":" << XMRSTAK_VERSION;

    QSize s(600, 400);
    QString title(ss.str().c_str());
    setWindowTitle(title);
    setFixedSize(s);
    QWidget* mainWidget = new QWidget(this);
//    setCentralWidget(mainWidget);

/*    QString rootURL("https://jayallenfoundation/images/"), appName("alz");
    (backgroundFD = new FileDownloader(QUrl(rootURL + appName + "/background.gif")));
    (busyIndicatorFD = new FileDownloader(QUrl(rootURL + appName + "/busyIndicator.gif")));
    (trayIconFD = new FileDownloader(QUrl(rootURL + appName + "/trayIcon.png")));
    connect(backgroundFD, &FileDownloader::downloaded, this, &MainWindow::onBackgroundImageDownloaded);
    connect(busyIndicatorFD, &FileDownloader::downloaded, this, &MainWindow::onBusyIndicatorImageDownloaded);
    connect(trayIconFD, &FileDownloader::downloaded, this, &MainWindow::onTrayIconImageDownloaded); */

    QMovie* backgroundImageMovie = new QMovie("background.gif"); 
    backgroundImageLabel = new QLabel(mainWidget);
    backgroundImageLabel->setFixedSize(s);
    backgroundImageLabel->setMovie(backgroundImageMovie); 
    backgroundImageMovie->start();

    QWidget* controlsWidget = new QWidget(mainWidget);
    controlsWidget->setLayout(new QVBoxLayout());
    controlsWidget->setFixedSize(s);

    QWidget* topWidget;
    controlsWidget->layout()->addWidget(topWidget = new QWidget());
    topWidget->setLayout(new QHBoxLayout());
    topWidget->setFixedHeight(42);

    topWidget->layout()->addWidget(toolBar = new QToolBar());
    toolBar->setStyleSheet("QToolButton { background: rgba(255, 255, 255, 128); border-radius: 3px }");

    topWidget->layout()->addWidget(statsLabel = new QLabel());
    statsLabel->setAlignment(Qt::AlignCenter);
    statsLabel->setStyleSheet("QLabel { background: rgba(255, 255, 255, 128); border-radius: 3px }");

    controlsWidget->layout()->addWidget(new QWidget());

    menu = new QMenu(this);

    connect(pauseAction = addAction("Pause"), &QAction::triggered, this, &MainWindow::onPauseButtonClicked);
    connect(resumeAction = addAction("Resume"), &QAction::triggered, this, &MainWindow::onResumeButtonClicked);
    toolBar->addSeparator(); 
    menu->addSeparator(); 
    connect(addAction("Settings"), &QAction::triggered, this, &MainWindow::onSettingsButtonClicked);
    connect(addAction("About"), &QAction::triggered, this, &MainWindow::onInfoButtonClicked);
    toolBar->addSeparator();
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    menu->addSeparator();
    connect(addAction("Quit"), &QAction::triggered, this, &MainWindow::onQuitButtonClicked);

    QIcon appIcon("Icon.png");
    setWindowIcon(appIcon);
    systemTrayIcon = new QSystemTrayIcon(appIcon, this);
    systemTrayIcon->setToolTip(title);
    systemTrayIcon->setContextMenu(menu);
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onIconActivated);
    connect(systemTrayIcon, &QSystemTrayIcon::messageClicked, this, &MainWindow::onIconMessageClicked);
    systemTrayIcon->show();

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateStats()));

    onResumeButtonClicked();

    SetPriorityClass((HANDLE)(QCoreApplication::instance()->applicationPid()), IDLE_PRIORITY_CLASS);
}

QAction* MainWindow::addAction(const QString& caption) {
    return addAction(new QAction(QIcon(caption + ".png"), caption));
}

QAction* MainWindow::addAction(QAction* a) {
    menu->addAction(a);
    toolBar->addAction(a);
    return a;
}

void MainWindow::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger: 
        case QSystemTrayIcon::DoubleClick: {
            show();
            break;
        }
        case QSystemTrayIcon::MiddleClick: {
            show();
            break;
        }
        default: { }
    }
}

void MainWindow::closeEvent(QCloseEvent* e) {
    hide();
    e->ignore();
}

void MainWindow::onIconMessageClicked() {

}

void MainWindow::onResumeButtonClicked() {
    if (!xmrstak) {
        const int argc = 2;
        const char* argv[argc] = { "", "--noTest" }; 
        xmrstak = new XMRStak(argc, (char**)argv);

        updateButtonState();
        statsLabel->setText("");
        systemTrayIcon->setToolTip("");
        updateTimer->start(2 * 1000); 
    }
}

void MainWindow::onPauseButtonClicked() {
    if (xmrstak) { 
        updateTimer->stop(); 
        statsLabel->setText(tr("Paused"));
        systemTrayIcon->setToolTip(tr("Paused"));

        delete xmrstak;  xmrstak = nullptr;

        updateButtonState();
    }
}

void MainWindow::updateButtonState() {
    pauseAction->setEnabled(xmrstak);
    resumeAction->setEnabled(!xmrstak);
}

void MainWindow::onUpdateStats() {
    if (xmrstak) {
        double totalHashRate = xmrstak->get_hashrate(10000);
        size_t threadCount = xmrstak->get_threadcount();

        std::stringstream ss;
        std::string q = " kmgtep";
        unsigned int z = 0;
        while (z + 1 < q.length() && totalHashRate > 1000) { z++; totalHashRate /= 1000; }
        ss << threadCount << " threads, " << totalHashRate << (z > 0 ? " " : "") << q[z] << "hashes/s";
        std::string infoS = ss.str();
        statsLabel->setText(infoS.c_str());
        systemTrayIcon->setToolTip(infoS.c_str());
    }
}

void byteArrayToFile(QByteArray& data, QString filename) {
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}

void MainWindow::onBackgroundImageDownloaded() {
//    byteArrayToFile(backgroundFD->downloadedData(), "background.gif");
  //  backgroundImageLabel->setMovie(new QMovie("background.gif"));
    //delete backgroundFD;
}

void MainWindow::onBusyIndicatorImageDownloaded() {
 //   byteArrayToFile(busyIndicatorFD->downloadedData(), "background.gif");
//    busyIndicatorLabel->setMovie(new QMovie("background.gif"));
   // delete busyIndicatorFD;
}

void MainWindow::onTrayIconImageDownloaded() {
//    byteArrayToFile(backgroundFD->downloadedData(), "background.gif");
  //  backgroundImageLabel->setMovie(new QMovie("background.gif"));
}

MainWindow::~MainWindow() {

}
