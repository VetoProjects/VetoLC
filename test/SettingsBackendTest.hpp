#ifndef SETTINGSBACKENDTEST
#define SETTINGSBACKENDTEST

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
    void settingsWriteTest(){
        SettingsBackend::saveSettingsFor(4711, QStringLiteral("testkey"), QVariant(25));
        SettingsBackend::saveSettingsFor(4711, QStringLiteral("testkey2"), QVariant("joy"));
    }
    void settingsReadTest(){
        QHash<QString, QVariant> settings = SettingsBackend::getSettings(4711);
        QVERIFY(!settings.empty());
        QVERIFY(settings.contains("testkey"));
        QVERIFY(settings.value("testkey").toInt() == 25);
        QVERIFY(settings.contains("testkey2"));
        QVERIFY(settings.value("testkey2").toString() == QStringLiteral("joy"));
    }
    void settingsRemoveTest(){
        SettingsBackend::removeSettings(4711);
    }
};

#endif // SETTINGSBACKENDTEST
