#ifndef SETTINGSBACKENDTEST
#define SETTINGSBACKENDTEST

#include <QObject>
#include <QTest>
#include "../src/SettingsBackend.hpp"

/**
 * @brief The SettingsBackend Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the SettingsBackend class; functionality tested includes
 * object creation, writing to and reading from the settings.
 */
class SettingsBackendTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        backend = new SettingsBackend();
    }
    void objectCreationTest(){
        QVERIFY(backend);
    }
    void settingsWriteTest(){
        backend->saveSettingsFor(4711, QString("testkey"), QVariant(25));
        backend->saveSettingsFor(4711, QString("testkey2"), QVariant("joy"));
    }
    void settingsReadTest(){
        QHash<QString, QVariant> settings = backend->getSettings(4711);
        QVERIFY(!settings.empty());
        QVERIFY(settings.contains("testkey"));
        QVERIFY(settings.value("testkey").toInt() == 25);
        QVERIFY(settings.contains("testkey2"));
        QVERIFY(settings.value("testkey2").toString() == QString("joy"));
    }
    void settingsRemoveTest(){
        backend->removeSettings(4711);
    }
    void cleanupTestCase(){
        delete backend;
    }
private:
    SettingsBackend *backend;
};

#endif // SETTINGSBACKENDTEST
