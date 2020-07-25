#include "MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSize>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSize s = QApplication::desktop()->geometry().size();
    double f = 2/3.0, g = (1 - f)/2;
    w.setGeometry(g*s.width(), g*s.height(), f*s.width(), f*s.height());
    w.show();

    return a.exec();
}
