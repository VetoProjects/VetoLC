#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>
#include <QTranslator>
#include <QFontDatabase>
#include <QDebug>
#include "EditorWindow.hpp"
#include "Renderer.hpp"
#include "BootLoader.hpp"
#include "Instances/WindowInstance.hpp"

static const QString socketName = "VeTo";

/**
 * @brief main
 * @param argc
 * @param argv
 * @return return code of the QApplication
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * The main class. Sets up the application and returns its' return code.
 */
int main(int argc, char *argv[]){
    QLocalSocket socket;
    socket.connectToServer(socketName);
    if(socket.waitForConnected(500)){
        socket.write("");
        return 0;
    }

    QApplication a(argc, argv);

    if(QFontDatabase::addApplicationFont(":/fonts/Inconsolata.otf") == -1)
        qWarning("Failed to load font Inconsolata.");

    QTranslator translator;
    translator.load(QString("codeeditor_") + QLocale::system().name());
    a.installTranslator(&translator);

    Backend server(&a);

    QList<int> ids = server.loadIds();
    for(int id : ids){
        QHash<QString,QVariant> settings = server.getSettings(id);
        Instances::WindowInstance *instance = new Instances::WindowInstance(id, settings, &server);
        server.addInstance(instance, false);
    }
    BootLoader *bootLoader = new BootLoader(socketName, &server);
    bootLoader->start();
    qDebug() << bootLoader;
    return a.exec();
}
