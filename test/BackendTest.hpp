#ifndef BACKENDTEST
#define BACKENDTEST

#include <QObject>
#include <QTest>
#include "../src/Backend.hpp"
#include "../src/Instances/WindowInstance.hpp"

/**
 * @brief The Backend Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the Backend class; functionality tested includes
 * object creation, instance creation and removing and
 * settings management.
 */
class BackendTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        backend = new Backend();
        instance = new WindowInstance(100, QHash<QString, QVariant>());
    }
    void objectCreationTest() {
        QVERIFY(backend);
    }
    void instanceCreationTest(){
        int ids = backend->loadIds().length();
        QList<WindowInstance*> list;
        for(int i = 0; i < ids; i++){
            list.append(new WindowInstance(i, QHash<QString, QVariant>()));
            backend->addInstance(list[i]);
        }
        backend->addInstance(instance);
        QVERIFY(ids + 1 == backend->loadIds().length());
        QVERIFY(backend->removeInstance(instance));
        QVERIFY(ids == backend->loadIds().length());
        QVERIFY(backend->loadIds().length() == 1 ? backend->isLast() : !backend->isLast());
        for(int i = 0; i < ids; i++)
            backend->removeInstance(list[i]);
    }
    void settingsTest(){
        instance = new WindowInstance(1, QHash<QString, QVariant>());
        backend->addInstance(instance);
        QList<int> ids = backend->loadIds();
        for(int id : ids){
            QVERIFY(id >= 0);
        }
        backend->removeSettings(instance);
    }
    void cleanupTestCase() {
        delete backend;
    }
private:
    Backend *backend;
    WindowInstance *instance;
};

#endif // BACKENDTEST
