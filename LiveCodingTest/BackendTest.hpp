#ifndef BACKENDTEST
#define BACKENDTEST

#include <QObject>
#include <QTest>
#include "../Backend.hpp"

class BackendTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        backend = new Backend();
    }
    void objectCreationTest() {
        QVERIFY(backend);
    }
    void cleanupTestCase() {
        delete backend;
    }
private:
    Backend *backend;
};


#endif // BACKENDTEST
