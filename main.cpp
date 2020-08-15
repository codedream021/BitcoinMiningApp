#include "MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSize>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow w;
    QSize s = QApplication::desktop()->geometry().size(), wSize = w.size();
    w.setGeometry((s.width() - wSize.width())/2, (s.height() - wSize.height())/2, wSize.width(), wSize.height());
    w.show();

    return a.exec();
}
