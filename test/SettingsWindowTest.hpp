#ifndef SETTINGSWINDOWTEST
#define SETTINGSWINDOWTEST

#include <QObject>
#include <QTest>
#include "../src/SettingsWindow.hpp"

class SettingsWindowTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        settingsWindow = new SettingsWindow(1);
    }
    void objectCreationTest() {
        QVERIFY(settingsWindow);
    }
    void cleanupTestCase() {
        delete settingsWindow;
    }
private:
    SettingsWindow *settingsWindow;
};


#endif // SETTINGSWINDOWTEST
