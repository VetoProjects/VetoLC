#include <QThread>
#include "BootLoader.hpp"
#include "QLocalSocket"
#include "EditorWindow.hpp"
#include "Instances/WindowInstance.hpp"

/**
 * @brief BootLoader::BootLoader
 * @param socketName
 * @param parent
 *
 * Constructor of the BootLoader class.
 * instantiates the socket and registers with the parent.
 */
BootLoader::BootLoader(const QString &socketName, Backend *parent) :
    QObject(parent), backend(parent), socketName(socketName)
{ }

/**
 * @brief BootLoader::~BootLoader
 *
 * Destructor of the Bootloader class.
 * Cleans up the server.
 */
BootLoader::~BootLoader(){
    delete server;
}

/**
 * @brief BootLoader::start
 *
 * starts the BootLoader/socket. runs asynchronously.
 */
void BootLoader::start()
{
    server = new QLocalServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    server->listen(socketName);
}


/**
 * @brief BootLoader::acceptConnection
 *
 * Accepts a connection from a client,
 * registers it with the backend(so it will run in the main app)
 * and kills the original client.
 */
void BootLoader::acceptConnection(){
    QLocalSocket *client = server->nextPendingConnection();
    QByteArray request = client->readAll();
    client->close();
    delete client;
    if(request.size() == 0){
        Instances::WindowInstance *instance = new Instances::WindowInstance(backend->nextID(), QHash<QString,QVariant>(), backend);
        backend->addInstance(instance);
    }
}
