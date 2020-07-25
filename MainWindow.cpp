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

#include "InfoDialog.h"

extern "C" {
//#include "cpuminer/miner.h"
}

void MainWindow::onInfoButtonClicked() { InfoDialog(this).exec(); }

void MainWindow::onStopMiningButtonClicked() {
    updateTimer->stop();
    statsLabel->setText("");

    startMiningButton->setEnabled(true);
    stopMiningButton->setEnabled(false);
    std::cout << "Stop mining clicked..." << std::endl;

//    finish_mining();

    std::cout << "Stopped mining." << std::endl;
}

void MainWindow::onStartMiningButtonClicked() {
    startMiningButton->setEnabled(false);
    stopMiningButton->setEnabled(true);
    std::cout << "Start mining clicked..." << std::endl;

    std::string threads = "1";
    std::vector<std::string> argvStrings;
    argvStrings.push_back("minerd");
    argvStrings.push_back("--algo=sha256d");
    argvStrings.push_back("--url=" + urlLineEdit->text().toStdString());
    argvStrings.push_back("--user=" + userLineEdit->text().toStdString());
    argvStrings.push_back("--pass=" + passLineEdit->text().toStdString());
    argvStrings.push_back("--threads=" + threads);
    const char* argv[6] = { argvStrings[0].c_str(), argvStrings[1].c_str(), argvStrings[2].c_str(), argvStrings[3].c_str() , argvStrings[4].c_str() , argvStrings[5].c_str() };//, argvStrings[6].c_str() };
//    cpuminer_main(argvStrings.size(), argv);

    updateTimer->start(10000);

    std::cout << "Miner started..." << std::endl;
}

QLineEdit* MainWindow::addLabeledEdit(const QString& labelText, const QString& initialContents) {
    QWidget* w; QLayout* l; QLineEdit* le;
    mainWidget->layout()->addWidget(w = new QWidget());
    w->setLayout(l = new QHBoxLayout());
    l->addWidget(new QLabel(labelText));
    l->addWidget(le = new QLineEdit(initialContents));
    return le;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(mainWidget = new QWidget(this));
    mainWidget->setLayout(new QVBoxLayout());

    mainWidget->layout()->addWidget(statsLabel = new QLabel());

    QWidget* widgetButtons;
    mainWidget->layout()->addWidget(widgetButtons = new QWidget());
    widgetButtons->setLayout(new QHBoxLayout());
    widgetButtons->layout()->addWidget(startMiningButton = new QPushButton(tr("Start mining")));
    widgetButtons->layout()->addWidget(stopMiningButton = new QPushButton(tr("Stop mining")));
    widgetButtons->layout()->addWidget(infoButton = new QPushButton(tr("Info")));
    stopMiningButton->setEnabled(false);
    connect(startMiningButton, SIGNAL(released()), this, SLOT(onStartMiningButtonClicked()));
    connect(stopMiningButton, SIGNAL(released()), this, SLOT(onStopMiningButtonClicked()));
    connect(infoButton, SIGNAL(released()), this, SLOT(onInfoButtonClicked()));

    userLineEdit = addLabeledEdit(tr("Username"), "core.001");
    passLineEdit = addLabeledEdit(tr("Password"), "123");
    urlLineEdit = addLabeledEdit(tr("Pool url"), "stratum+tcp://btc.ss.poolin.com:443");

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateStats()));
}

void MainWindow::onUpdateStats() {
    /*
    int numThreads = get_num_threads();
    double totalHashRate = 0.0;
    for (int x = 0; x < numThreads; ++x) totalHashRate += get_hashrate_for_thread(x);

    std::stringstream ss;
    std::string q = " kmgtep";
    unsigned int z = 0;
    while (z + 1 < q.length() && totalHashRate > 1000) { z++; totalHashRate /= 1000; }
    ss << "Threads: " << numThreads << ", hashrate: " << totalHashRate << (z > 0 ? " " : "") << q[z] << "hashes/s";
    statsLabel->setText(ss.str().c_str());
    */
}

MainWindow::~MainWindow()
{

}
