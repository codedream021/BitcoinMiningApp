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
#include <QTimer>
#include <QMovie>
#include <QCoreApplication>

#include <Windows.h>
//#include <processthreadsapi.h>

#include "InfoDialog.h"

#include "xmrstak.h"

//#include "cpuminer/miner.h"


void MainWindow::onInfoButtonClicked() { InfoDialog(this).exec(); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(mainWidget = new QWidget(this));
    mainWidget->setLayout(new QVBoxLayout());

    QLabel* busyIndicatorLabel = new QLabel();
    QMovie* busyIndicatorMovie = new QMovie("active.gif");
    mainWidget->layout()->addWidget(busyIndicatorLabel);
    busyIndicatorLabel->setMovie(busyIndicatorMovie);
    busyIndicatorMovie->start();

    mainWidget->layout()->addWidget(statsLabel = new QLabel()); 

    QPushButton* infoDialogButton;
    mainWidget->layout()->addWidget(infoDialogButton = new QPushButton(tr("&Info")));
    connect(infoDialogButton, SIGNAL(released()), this, SLOT(onInfoButtonClicked()));

//    std::string threads = "1";
    std::vector<std::string> argvStrings;
    argvStrings.push_back("minerd");
    argvStrings.push_back("--noTest");
    /*    argvStrings.push_back("--algo=sha256d");
    argvStrings.push_back("--url=" + urlLineEdit->text().toStdString());
    argvStrings.push_back("--user=" + userLineEdit->text().toStdString());
    argvStrings.push_back("--pass=" + passLineEdit->text().toStdString());
    argvStrings.push_back("--threads=" + threads);

 */
 //    const char* argNoTest = "--noTest";
    const char* argv[2] = { argvStrings[0].c_str() , argvStrings[1].c_str() };// , argvStrings[2].c_str(), argvStrings[3].c_str(), argvStrings[4].c_str(), argvStrings[5].c_str()
//, argvStrings[6].c_str() }; 
    SetPriorityClass((HANDLE) (QCoreApplication::instance()->applicationPid()), IDLE_PRIORITY_CLASS);

    xmrstak_main(argvStrings.size(), (char**)argv); 

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateStats()));
    updateTimer->start(2*1000);
}

void MainWindow::onUpdateStats() {
    double totalHashRate = get_hashrate(10000);
    size_t threadCount = get_threadcount(); 


    std::stringstream ss;
    std::string q = " kmgtep";
    unsigned int z = 0;
    while (z + 1 < q.length() && totalHashRate > 1000) { z++; totalHashRate /= 1000; }
    ss << threadCount << " threads, " << totalHashRate << (z > 0 ? " " : "") << q[z] << "hashes/s";
    statsLabel->setText(ss.str().c_str());
}

MainWindow::~MainWindow()
{

}
