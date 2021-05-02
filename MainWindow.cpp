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
#include <QPixmap>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFile>
#include <QString>
#include <QTextCodec>

#include <Windows.h>
//#include <processthreadsapi.h>

#include "InfoDialog.h"
#include "SettingsDialog.h"
#include "FileDownloader.h"

#include "xmrstak.h"

//#include "cpuminer/miner.h"
QString rootURL("https://safehouse-cybertrust.github.io/");
const long long version = atoll(GUI_VERSION);// << 16) | atoll(XMRSTAK_VERSION);

bool performUpdate = false;

std::string GetLastErrorAsString() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) return std::string(); //No error message has been recorded
    
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
}

VOID startup(LPCTSTR lpApplicationName, LPSTR arguments) {
    std::cout << "Starting update... ";
    STARTUPINFO si;     
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    bool success = CreateProcess( lpApplicationName, arguments, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) != 0;
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    std::cout << "Updater terminated. ";
    if (!success) std::cout << "Error: " << GetLastErrorAsString() << ".";
}

void MainWindow::onInfoButtonClicked() { InfoDialog(this).exec(); }
void MainWindow::onSettingsButtonClicked() { 
    SettingsDialog d(this, loadFraction);
    if (d.exec() == QDialog::Accepted) {
        loadFraction = d.loadFraction();
        if (xmrstak) { onPauseButtonClicked(); onResumeButtonClicked(); }
    }; 
}

void MainWindow::onQuitButtonClicked() {
    onPauseButtonClicked();
    updateTimer->stop();
    systemTrayIcon->hide();
    delete systemTrayIcon;
    QCoreApplication::quit();
}

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    std::stringstream ss; ss << "EPU v" << GUI_VERSION << " - E Pluribus Unum–Out of Many, One";

    std::cout << "Starting version " << GUI_VERSION << std::endl;

    QSize s(600, 400);
    QString title(ss.str().c_str());
    setWindowTitle(title);
    setFixedSize(s);
    QWidget* mainWidget = new QWidget(this);

    QPixmap pix("background.jpg");
    backgroundImageLabel = new QLabel(mainWidget);
    backgroundImageLabel->setFixedSize(s);
    backgroundImageLabel->setPixmap(pix.scaled(s.width(), s.height()));  

    QWidget* controlsWidget = new QWidget(mainWidget);
    controlsWidget->setLayout(new QVBoxLayout());
    controlsWidget->setFixedSize(s);


    QWidget *topSpacer, *botWidget; 
    controlsWidget->layout()->setMargin(0);
    controlsWidget->layout()->setSpacing(0);
    controlsWidget->layout()->addWidget(topSpacer = new QWidget());
    controlsWidget->layout()->addWidget(toolBarWidget = new QWidget());
    controlsWidget->layout()->addWidget(botWidget = new QWidget());
    controlsWidget->layout()->addWidget(new QWidget());
    topSpacer->setFixedHeight((182*s.height())/1620);
    toolBarWidget->setFixedHeight((121*s.height())/1620);
    botWidget->setFixedHeight((1200*s.height())/1620);

    toolBarWidget->setLayout(new QHBoxLayout());
    toolBarWidget->layout()->setMargin(0);
    toolBarWidget->layout()->setSpacing(0);
    botWidget->setLayout(new QHBoxLayout());
    botWidget->layout()->setMargin(0);
    botWidget->layout()->setSpacing(0);

    toolBarWidget->setStyleSheet("QPushButton { background: rgba(255, 25, 255, 0) }");

    botWidget->layout()->addWidget(statsLabel = new QLabel());
    statsLabel->setAlignment(Qt::AlignCenter);
//    statsLabel->setStyleSheet("QLabel { background: rgba(255, 255, 255, 68); border-radius: 3px }");
    statsLabel->setFixedWidth((1119*s.width())/2400);
    botWidget->layout()->addWidget(new QWidget());

    menu = new QMenu(this);

    connect(pauseAction = addAction("Pause"), &QAction::triggered, this, &MainWindow::onPauseButtonClicked);
    connect(resumeAction = addAction("Resume"), &QAction::triggered, this, &MainWindow::onResumeButtonClicked);
    menu->addSeparator(); 
    connect(addAction("Settings"), &QAction::triggered, this, &MainWindow::onSettingsButtonClicked);
    connect(addAction("About"), &QAction::triggered, this, &MainWindow::onInfoButtonClicked);
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

    (latestVersionFD = new FileDownloader(QUrl(rootURL + "updates/version.txt")));
    connect(latestVersionFD, &FileDownloader::downloaded, this, &MainWindow::onLatestVersionDownloaded);
}

QAction* MainWindow::addAction(const QString& caption) {
    return addAction(new QAction(QIcon(caption + ".png"), caption));
}

QAction* MainWindow::addAction(QAction* a) {
    menu->addAction(a);
    QPushButton* button;
    toolBarWidget->layout()->addWidget(button = new QPushButton());
    connect(button, &QPushButton::clicked, a, &QAction::triggered);
//    toolBar->addAction(a);
    return a;
}

void MainWindow::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger: 
        case QSystemTrayIcon::DoubleClick:  
        case QSystemTrayIcon::MiddleClick: { show(); break; }
        default: { }
    }
}

void MainWindow::closeEvent(QCloseEvent* e) { hide(); e->ignore(); }

void MainWindow::onIconMessageClicked() {

}

void MainWindow::onResumeButtonClicked() {
    if (!xmrstak) {
        const int argc = 2;
        const char* argv[argc] = { "", "--noTest" }; 
        xmrstak = new XMRStak(argc, (char**)argv, loadFraction);

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

void MainWindow::onLatestVersionDownloaded() {
    std::string latestVersionString = QTextCodec::codecForMib(106)->toUnicode(latestVersionFD->downloadedData()).toStdString();
    std::cout << "Latest version available: " << latestVersionString << "\n";
    long long latestVersion = atoll(latestVersionString.c_str());
    if (version < latestVersion) {
        std::cout << "\nDownloading update...";
        (updaterFD = new FileDownloader(QUrl(rootURL + "updates/eps-updater.exe")));
        connect(updaterFD, &FileDownloader::downloaded, this, &MainWindow::onUpdaterDownloaded);
    }
}

void MainWindow::onUpdaterDownloaded() {
    std::cout << "\nUpdate downloaded.";
    byteArrayToFile(updaterFD->downloadedData(), "eps-updater.exe");
    performUpdate = true;
    startup("eps-updater.exe", "");
}

MainWindow::~MainWindow() {

}
