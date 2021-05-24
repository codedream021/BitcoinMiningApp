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
#include <QSettings>
#include <QFileInfo>
#include <QDir>

#ifdef WIN32
#include <Windows.h>
#endif

#include "FileDownloader.h"

#include "unrar.hpp"

QString rootURL("https://safehouse-cybertrust.github.io/");

bool performUpdate = false;

#ifdef WIN32
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

HANDLE hAppMutex;
#endif

MainWindow::MainWindow(bool iSilent, bool iFirstRun, QWidget *parent) : QDialog(parent), silent(iSilent), firstRun(iFirstRun) {
    // Single instance mutex
#ifdef WIN32
    hAppMutex = CreateMutex(NULL, TRUE, (LPCSTR) "safehouse-cybertrust-EPU-launcher");
    if (GetLastError() == ERROR_ALREADY_EXISTS) { QCoreApplication::quit(); exit(0); return; }
#endif

    // Go to executable path
    QDir::setCurrent(QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath());
 
    // rest of the program
    std::stringstream ss; ss << "E Pluribus Unum - Launcher";

    std::cout << "Starting version " << VERSION << std::endl;

    QSize s(300, 200);
    QString title(ss.str().c_str());
    setWindowTitle(title);
    setFixedSize(s);
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(new QVBoxLayout());
    mainWidget->layout()->addWidget(msgLabel = new QLabel());

    msgLabel->setText("Checking for updates...");

    (latestVersionFD = new FileDownloader(QUrl(rootURL + "updates/version.txt")));
    connect(latestVersionFD, &FileDownloader::downloaded, this, &MainWindow::onLatestVersionDownloaded);
    connect(latestVersionFD, &FileDownloader::error, this, &MainWindow::onUpdaterDownloadError);
}

void MainWindow::closeEvent(QCloseEvent* e) { hide(); e->ignore(); }

void byteArrayToFile(const QByteArray& data, QString filename) {
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}

void MainWindow::onLatestVersionDownloaded() {
    std::string latestVersionString = QTextCodec::codecForMib(106)->toUnicode(latestVersionFD->downloadedData()).toStdString();
    std::cout << "Latest version available: " << latestVersionString << "\n";
    long long latestVersion = atoll(latestVersionString.c_str());

    QFile file("version.txt");
    long long version = 0;
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString line = in.readLine();
        version = atoll(line.toStdString().c_str());
        file.close();
    }
    std::cout << "Installed version: " << version << "\n";

    if (version < latestVersion) {
        msgLabel->setText("Downloading update...");

        std::cout << "\nDownloading update..." << std::endl;
        (updaterFD = new FileDownloader(QUrl(rootURL + "updates/update.dat")));
        connect(updaterFD, &FileDownloader::downloaded, this, &MainWindow::onUpdaterDownloaded);
        connect(updaterFD, &FileDownloader::error, this, &MainWindow::onUpdaterDownloadError);
    } else launchApp();
}

void MainWindow::onUpdaterDownloadError(std::string e) {
    std::cout << "\nUpdate download error..." << std::endl;
    launchApp();
}

void MainWindow::onUpdaterDownloaded() {
    msgLabel->setText("Updating...");
    std::string updaterName = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath().toStdString() + std::string("EPluribusUnum-Updater.rar");
    std::cout << "\nUpdate downloaded. Saving to " << updaterName << std::endl;
    byteArrayToFile(updaterFD->downloadedData(), updaterName.c_str());
    performUpdate = true;
    // extract
    std::cout << "\nInstalling update..." << std::endl;
    ExtractArchive((char*) updaterName.c_str(), 0);
    launchApp();
}

const QString appStartKey = "E-Pluribus-Unum";

void MainWindow::launchApp() {
#ifdef WIN32
    msgLabel->setText("Launching...");
    std::string args = "";
    if (silent) args += " /silent";
    if (firstRun) args += " /firstRun";
    std::cout << "\nLaunching app with args '" << args << "'..." << std::endl;
    std::string appName = "E-Pluribus-Unum.exe";
    startup(appName.c_str(), (char*) (appName + args).c_str());
    std::cout << "\nQuitting..." << std::endl;
    close();
    QCoreApplication::quit();
#endif
}

MainWindow::~MainWindow() {
#ifdef WIN32
    ReleaseMutex(hAppMutex);
    CloseHandle(hAppMutex);
#endif
}
 
