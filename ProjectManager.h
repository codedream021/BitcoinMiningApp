#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QDebug>

//class QWidget;
class QLabel;
class QTimer;
class QPushButton;
class QAction;
class QMovie;
class QSystemTrayIcon;
class QMenu;
class QToolBar;

class FileDownloader;

class XMRStak;

class ProjectManager : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectManager(bool firstRun, QWidget *parent = nullptr);
    ~ProjectManager();
    Q_PROPERTY(QString windowTitle MEMBER m_windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged);
    Q_PROPERTY(QString statsText MEMBER m_statsText WRITE setStatsText NOTIFY statsTextChanged);
    Q_PROPERTY(bool uiVisible MEMBER m_uiVisible WRITE setUiVisible NOTIFY uiVisibleChanged);
    Q_PROPERTY(bool running MEMBER m_running NOTIFY runningChanged);
    Q_INVOKABLE void setUiVisible(bool visible);

signals:
    void windowTitleChanged();
    void statsTextChanged();
    void uiVisibleChanged();
    void buttonClicked(int index);
    void runningChanged();

private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onIconMessageClicked();
    void onUpdateStats();
    void onPauseButtonClicked();
    void onResumeButtonClicked();
    void onSettingsButtonClicked();
    void onInfoButtonClicked();
    void onQuitButtonClicked();
    void onButtonClicked(int index);

private:
    XMRStak* m_xmrstak = nullptr;
    double m_loadFraction = 0.2;
    bool m_autoStart = false;
    QString m_windowTitle;
    QString m_statsText;
    QMenu* m_menu;
    QAction *m_pauseAction, *m_resumeAction;
    QSystemTrayIcon* m_systemTrayIcon;
    QTimer *m_updateTimer;
    bool m_uiVisible = false;
    bool m_running = false;

    bool getAutoStart();
    void setAutoStart(bool autoStart);
    void writePoolFile();
    std::string generateID();
    QAction* addAction(QAction* a);
    QAction* addAction(const QString& caption);
    void setWindowTitle(QString title);
    void setStatsText(QString stats);
    void updateButtonState();
};

#endif // PROJECTMANAGER_H

