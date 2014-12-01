#ifndef BACKENDTEST
#define BACKENDTEST

#include <memory>

#include <QTest>

#include "../src/Backend.hpp"
#include "../src/Instances/WindowInstance.hpp"

/**
 * @brief The Backend Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the Backend class; functionality tested includes
 * object creation, instance creation and removing, settings
 * management and finding directories.
 */
class BackendTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        backend = std::unique_ptr<Backend>(new Backend());
        instance = std::unique_ptr<WindowInstance>(new WindowInstance(100, QHash<QString, QVariant>()));
    }
    void objectCreationTest() {
        QVERIFY(backend.get());
    }
    void instanceCreationTest(){
        int ids = backend->loadIds().length();
        QList<WindowInstance*> list;
        for(int i = 0; i < ids; i++){
            list.append(new WindowInstance(i, QHash<QString, QVariant>()));
            backend->addInstance(list[i]);
        }
        backend->addInstance(instance.get());
        QVERIFY(ids + 1 == backend->loadIds().length());
        QVERIFY(backend->removeInstance(instance.get()));
        QVERIFY(ids == backend->loadIds().length());
        QVERIFY(backend->loadIds().length() == 1 ? backend->isLast() : !backend->isLast());
        for(int i = 0; i < ids; i++)
            backend->removeInstance(list[i]);
    }
    void settingsTest(){
        instance = std::unique_ptr<WindowInstance>(new WindowInstance(1, QHash<QString, QVariant>()));
        backend->addInstance(instance.get());
        QList<int> ids = backend->loadIds();
        for(int id : ids){
            QVERIFY(id >= 0);
        }
        backend->removeSettings(instance.get());
    }
    void dirTest(){
        QDir dir = Backend::directoryOf(QStringLiteral("/translations"));
        QVERIFY(dir.exists());
        dir = Backend::directoryOf(QStringLiteral("/dirDoesNotExist"));
        QVERIFY(!dir.exists());
    }
private:
    std::unique_ptr<Backend> backend;
    std::unique_ptr<WindowInstance> instance;
};

#endif // BACKENDTEST
