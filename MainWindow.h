#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class QLabel;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots: 
    void onInfoButtonClicked();
    void onUpdateStats();
private:
    QTimer* updateTimer;
    QWidget* mainWidget;
    QLabel  *statsLabel;
};

#endif // MAINWINDOW_H
