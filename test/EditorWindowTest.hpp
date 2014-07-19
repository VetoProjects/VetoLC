#ifndef EDITORWINDOWTEST
#define EDITORWINDOWTEST

#include <QObject>
#include <QTest>
#include "../src/EditorWindow.hpp"
#include "../src/SettingsBackend.hpp"

/**
 * @brief The EditorWindowTest class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the EditorWindow class; functionality tested includes
 * object creation.
 */
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
        delete editorWindow;
        delete backend;
    }
private:
    EditorWindow* editorWindow;
    SettingsBackend* backend;
};
#endif // EDITORWINDOWTEST
