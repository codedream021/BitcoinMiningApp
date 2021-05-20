#include "MainWindow.h"

#include <vector>
#include <string>

#include <QApplication>
#include <QDesktopWidget>
#include <QSize>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);

    std::vector<std::string> args;
    for (int q = 1; q < argc; ++q) args.push_back(argv[q]);

    bool silent = false, firstRun = false;
    for (int q = 0; q < args.size(); ++q) {
        if (args[q] == std::string("/silent")) silent = true;
        if (args[q] == std::string("/firstRun")) firstRun = true;
    }

    MainWindow w(silent, firstRun);
    QSize s = QApplication::desktop()->geometry().size(), wSize = w.size();
    w.setGeometry((s.width() - wSize.width())/2, (s.height() - wSize.height())/2, wSize.width(), wSize.height());
    if (!silent) w.show();

    return a.exec();
}