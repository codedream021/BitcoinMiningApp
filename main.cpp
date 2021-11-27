#include "ProjectManager.h"

#include <vector>
#include <string>
#include <iostream>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDesktopWidget>
#include <QSize>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("epprs");
    a.setOrganizationDomain("epprs.exe");
    QApplication::setQuitOnLastWindowClosed(false);

    std::vector<std::string> args;
    for (int q = 1; q < argc; ++q) args.push_back(argv[q]);

    bool silent = false, firstRun = false;
    std::cout << "Args = ";
    for (size_t q = 0; q < args.size(); ++q) { std::cout << "'" << args[q] << "', ";
        if (args[q] == std::string("/silent")) silent = true;
        if (args[q] == std::string("/firstRun")) firstRun = true;
    }
    std::cout << std::endl;

    firstRun = firstRun || !QFile::exists(QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath().append("/pools.txt"));

    std::cout << "Silent = " << silent << " firstRun = " << firstRun << std::endl;

    ProjectManager project(firstRun);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("project", &project);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    if(!silent) {
        project.setUiVisible(true);
    }

    return a.exec();
}
