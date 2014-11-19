#ifndef BOOTLOADER_HPP
#define BOOTLOADER_HPP

#include <QObject>
#include <QLocalServer>
#include <QThread>
#include <QLocalSocket>

#include "Instances/WindowInstance.hpp"
#include "Backend.hpp"

/**
 * @brief The BootLoader class
 *
 * A basic server that listens to new instances of
 * the program being started and registering them
 * with the main application.
 */
class BootLoader : public QObject
{
    Q_OBJECT
private:
    QLocalServer *server;
    Backend *backend;
    QString socketName;
public:
    explicit BootLoader(const QString &socketName, Backend *parent = 0);
    ~BootLoader();
    void start();

public Q_SLOTS:
    void acceptConnection();
};

#endif // BOOTLOADER_HPP
