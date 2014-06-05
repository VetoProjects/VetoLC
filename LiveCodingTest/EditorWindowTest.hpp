#ifndef EDITORWINDOWTEST
#define EDITORWINDOWTEST

#include <QObject>
#include <QTest>
#include "../EditorWindow.hpp"
#include "../SettingsBackend.hpp"

class EditorWindowTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        backend = new SettingsBackend();
        editorWindow = new EditorWindow(backend->getSettings(0));
    }
    void objectCreationTest() {
        QVERIFY(editorWindow);
    }
    void cleanupTestCase() {
        delete backend;
    }
private:
    EditorWindow* editorWindow;
    SettingsBackend* backend;
};
#endif // EDITORWINDOWTEST
