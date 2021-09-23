#include "ProjectManager.h"

#include <iostream>
#include <sstream>

#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QRandomGenerator64>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>

#ifdef WIN32
#include <Windows.h>
#endif

#include "InfoDialog.h"
#include "SettingsDialog.h"

#include "xmrstak.h"

QString rootURL("https://safehouse-cybertrust.github.io/");
const long long version = atoll(GUI_VERSION);// << 16) | atoll(XMRSTAK_VERSION);

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
#endif

#ifdef WIN32
HANDLE hAppMutex;
#endif
ProjectManager::ProjectManager(bool firstRun, QWidget *parent) : QWidget(parent)
{
    qDebug() << tr("ProjectManager::ProjectManager: firstRun = %1").arg(firstRun);
    // Single instance mutex
#ifdef WIN32
    hAppMutex = CreateMutex(NULL, TRUE, (LPCSTR) "safehouse-cybertrust");
    if (GetLastError() == ERROR_ALREADY_EXISTS) { QCoreApplication::quit(); exit(0); return; }
#endif
    // Go to executable path
    QDir::setCurrent(QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath());

    std::cout << "Current dir = " << QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath().toStdString() << std::endl;

    if (firstRun) { setAutoStart(true); writePoolFile(); }
    m_autoStart = getAutoStart();

    // rest of the program

    std::stringstream ss;
    ss << "EPU v" << GUI_VERSION << " - E Pluribus Unum–Out of Many, One";

    std::cout << "Starting version " << GUI_VERSION << std::endl;

    setWindowTitle(ss.str().c_str());

    connect(this, &ProjectManager::buttonClicked, this, &ProjectManager::onButtonClicked);

    m_menu = new QMenu(this);

    connect(m_pauseAction = addAction("Pause"), &QAction::triggered, this, &ProjectManager::onPauseButtonClicked);
    connect(m_resumeAction = addAction("Resume"), &QAction::triggered, this, &ProjectManager::onResumeButtonClicked);
    m_menu->addSeparator();
    connect(addAction("Settings"), &QAction::triggered, this, &ProjectManager::onSettingsButtonClicked);
    connect(addAction("About"), &QAction::triggered, this, &ProjectManager::onInfoButtonClicked);
    m_menu->addSeparator();
    connect(addAction("Quit"), &QAction::triggered, this, &ProjectManager::onQuitButtonClicked);

    QIcon appIcon(":/images/icon.png");
    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setIcon(appIcon);
    m_systemTrayIcon->setToolTip(m_windowTitle);
    m_systemTrayIcon->setContextMenu(m_menu);
    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &ProjectManager::onIconActivated);
    connect(m_systemTrayIcon, &QSystemTrayIcon::messageClicked, this, &ProjectManager::onIconMessageClicked);
    m_systemTrayIcon->show();

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &ProjectManager::onUpdateStats);

    onResumeButtonClicked();

#ifdef WIN32
    SetPriorityClass((HANDLE)(QCoreApplication::instance()->applicationPid()), IDLE_PRIORITY_CLASS);
#endif
}

ProjectManager::~ProjectManager()
{
#ifdef WIN32
    ReleaseMutex(hAppMutex);
    CloseHandle(hAppMutex);
#endif
}

void ProjectManager::setUiVisible(bool visible)
{
    m_uiVisible = visible;
    emit uiVisibleChanged();
}

void ProjectManager::onPauseButtonClicked()
{
    if (m_xmrstak) {
        m_updateTimer->stop();
        setStatsText(tr("Paused"));
        m_systemTrayIcon->setToolTip(tr("Paused"));

        delete m_xmrstak;  m_xmrstak = nullptr;

        updateButtonState();
    }
}

void ProjectManager::onResumeButtonClicked()
{
    if (!m_xmrstak) {
        const int argc = 2;
        const char* argv[argc] = { "", "--noTest" };
        std::cout << "Resuming with loadFraction = " << m_loadFraction << std::endl;
        m_xmrstak = new XMRStak(argc, (char**)argv, m_loadFraction);

        updateButtonState();
        setStatsText("");
        m_systemTrayIcon->setToolTip("");
        m_updateTimer->start(2 * 1000);
    }
}

void ProjectManager::onSettingsButtonClicked()
{
    SettingsDialog d(this, m_loadFraction, m_autoStart);
    if (d.exec() == QDialog::Accepted) {
        bool changed = false;
        if (m_loadFraction != d.loadFraction()) { changed = true; m_loadFraction = d.loadFraction(); }
        if (m_autoStart != d.autoStart()) { m_autoStart = d.autoStart(); setAutoStart(m_autoStart); }

        if (changed && m_xmrstak) { onPauseButtonClicked(); onResumeButtonClicked(); }
    };
}

void ProjectManager::onInfoButtonClicked()
{
    InfoDialog(this).exec();
}

void ProjectManager::onQuitButtonClicked()
{
    onPauseButtonClicked();
    m_updateTimer->stop();
    m_systemTrayIcon->hide();
    delete m_systemTrayIcon;
    QCoreApplication::quit();
}

void ProjectManager::onButtonClicked(int index)
{
    switch(index) {
    case 0:
        onPauseButtonClicked();
        break;
    case 1:
        onResumeButtonClicked();
        break;
    case 2:
        onSettingsButtonClicked();
        break;
    case 3:
        onInfoButtonClicked();
        break;
    case 4:
        onQuitButtonClicked();
        break;
    default:
        break;
    }
}

void ProjectManager::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick: { setUiVisible(true); break; }
        default: { }
    }
}

void ProjectManager::onIconMessageClicked()
{
    qDebug() << "ProjectManager::onIconMessageClicked";
}

void ProjectManager::onUpdateStats()
{
    if (m_xmrstak) {
        double totalHashRate = m_xmrstak->get_hashrate(10000);
        size_t threadCount = m_xmrstak->get_threadcount();

        std::stringstream ss;
        std::string q = " kmgtep";
        unsigned int z = 0;
        while (z + 1 < q.length() && totalHashRate > 1000) { z++; totalHashRate /= 1000; }
        ss << threadCount << " threads, " << totalHashRate << (z > 0 ? " " : "") << q[z] << "hashes/s";
        std::string infoS = ss.str();
        setStatsText(infoS.c_str());
        m_systemTrayIcon->setToolTip(infoS.c_str());
    }
}

const QString appStartKey = "E-Pluribus-Unum";

bool ProjectManager::getAutoStart()
{
#ifdef WIN32
    QSettings s("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    return s.contains(appStartKey);
#endif
}

void ProjectManager::setAutoStart(bool autoStart)
{
#ifdef WIN32
    std::cout << "Setting autostart = " << autoStart << std::endl;
    QSettings s("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString path = "\"" + QCoreApplication::applicationFilePath().replace("/", "\\") + "\" /silent";
    if (autoStart) { s.setValue(appStartKey, path);  std::cout << "SetValue = " << appStartKey.toStdString() << ": " << path.toStdString() << std::endl; } else { s.remove(appStartKey); }
#endif
}

void ProjectManager::writePoolFile()
{
    std::stringstream ss;
    ss << "\"pool_list\" : [{\"pool_address\" : \"stratum+tcp://pool.minexmr.com:4444\","
    << "\"wallet_address\" : \"47V3AhUsxRoGMJ3Qukg8Y59xYYG4sQE9o52uFNoVQ4BR3vGzZjzuAFrgVzLT9QkDirZyk1b388fKgeA5JupsfLEiDHnQUiJ\", "
    << "\"rig_id\" : \"" << generateID() << "\", \"pool_password\" : \"x\", \"use_nicehash\" : false, \"use_tls\" : false, \"tls_fingerprint\" : \"\", "
    << "\"pool_weight\" : 1 },], \"currency\" : \"randomx\",";
    std::string s = ss.str();
    QFile file("pools.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << QString(s.c_str());
    file.close();
}

std::string ProjectManager::generateID()
{
    QRandomGenerator& rg = *QRandomGenerator::global();
    std::string alf = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::string result = "";
    for (int q = 0; q < 19; ++q) result += alf[rg.bounded((quint32) (alf.length() - 1))];
    std::cout << "Generated ID: " << result << std::endl;
    return result;
}

QAction *ProjectManager::addAction(QAction *a)
{
    m_menu->addAction(a);
    return a;
}

QAction *ProjectManager::addAction(const QString &caption)
{
    return addAction(new QAction(QIcon(QString(":/images/%1.png").arg(caption)), caption));
}

void ProjectManager::setWindowTitle(QString title)
{
    m_windowTitle = title;
    emit windowTitleChanged();
}

void ProjectManager::setStatsText(QString stats)
{
    m_statsText = stats;
    emit statsTextChanged();
}

void ProjectManager::updateButtonState()
{
    m_pauseAction->setEnabled(m_xmrstak);
    m_resumeAction->setEnabled(!m_xmrstak);
}
