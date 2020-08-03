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

#include <Windows.h>
//#include <processthreadsapi.h>

#include "InfoDialog.h"

#include "xmrstak.h"

//#include "cpuminer/miner.h"


void MainWindow::onInfoButtonClicked() { InfoDialog(this).exec(); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSize s(600, 400);
    QString title("EPU Power Co-op");
    setWindowTitle(title);
    setFixedSize(s);
    setCentralWidget(mainWidget = new QWidget(this));

    QLabel* backgroundImageLabel;
    QMovie* backgroundImageMovie = new QMovie("background.gif");
    std::cout << "Background movie valid = " << (backgroundImageMovie->isValid() ? "yes" : "no");
    //mainWidget->layout()->addWidget(backgroundImageLabel = new QLabel());
    backgroundImageLabel = new QLabel(mainWidget);
    backgroundImageLabel->setFixedSize(s);
    backgroundImageLabel->setMovie(backgroundImageMovie);
    backgroundImageMovie->start();

    QWidget* controlsWidget = new QWidget(mainWidget);
    controlsWidget->setLayout(new QVBoxLayout());
    controlsWidget->setFixedSize(s);

    QSystemTrayIcon* sti = new QSystemTrayIcon(this);
    sti->setToolTip(title);

    controlsWidget->layout()->addWidget(statsLabel = new QLabel());
    statsLabel->setAlignment(Qt::AlignCenter);

    controlsWidget->layout()->addWidget(busyIndicatorLabel = new QLabel());
    busyIndicatorLabel->setMovie(busyIndicatorMovie = new QMovie("active.gif"));

    QToolBar* toolBar;
    controlsWidget->layout()->addWidget(toolBar = new QToolBar());
    QMenu* menu = new QMenu(this);

    toolBar->addAction(pauseAction = new QAction("&Pause"));
    menu->addAction(pauseAction);
    connect(pauseAction, &QAction::triggered, this, &MainWindow::onPauseButtonClicked);
    toolBar->addAction(resumeAction = new QAction("&Resume"));
    menu->addAction(resumeAction);
    connect(resumeAction, &QAction::triggered, this, &MainWindow::onResumeButtonClicked);
    toolBar->addAction(infoAction = new QAction("&Info"));
    menu->addAction(infoAction);
    connect(infoAction, &QAction::triggered, this, &MainWindow::onInfoButtonClicked);
    sti->setContextMenu(menu);
    sti->show();

    updateTimer = new QTimer(this);
    //connect(updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateStats()));

    onResumeButtonClicked();

    SetPriorityClass((HANDLE)(QCoreApplication::instance()->applicationPid()), IDLE_PRIORITY_CLASS);
}

void MainWindow::onResumeButtonClicked() {
    if (paused) { 
        std::vector<std::string> argvStrings;
        argvStrings.push_back("");
        argvStrings.push_back("--noTest");  
        const char* argv[2] = { argvStrings[0].c_str() , argvStrings[1].c_str() }; 

        xmrstak = new XMRStak();
        xmrstak->_main(argvStrings.size(), (char**)argv);

        paused = false;
        updateButtonState();
        updateTimer->start(2 * 1000);
        busyIndicatorMovie->start();
        busyIndicatorLabel->setVisible(true);
    }
}

void MainWindow::onPauseButtonClicked() {
    if (!paused) { 
        paused = true;
        updateTimer->stop();
        busyIndicatorMovie->stop();
        busyIndicatorLabel->setVisible(false);
        statsLabel->setText(tr("Paused"));

        xmrstak->terminate();
        delete xmrstak;
        xmrstak = nullptr;

        updateButtonState();
    }
}

void MainWindow::updateButtonState() {
    pauseAction->setEnabled(!paused);
    resumeAction->setEnabled(paused);
}

void MainWindow::onUpdateStats() {
    if (!paused) {
        double totalHashRate = xmrstak->get_hashrate(10000);
        size_t threadCount = xmrstak->get_threadcount();

        std::stringstream ss;
        std::string q = " kmgtep";
        unsigned int z = 0;
        while (z + 1 < q.length() && totalHashRate > 1000) { z++; totalHashRate /= 1000; }
        ss << threadCount << " threads, " << totalHashRate << (z > 0 ? " " : "") << q[z] << "hashes/s";
        statsLabel->setText(ss.str().c_str());
    }
}

MainWindow::~MainWindow() {

}
