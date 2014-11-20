#ifndef EDITORWINDOWTEST
#define EDITORWINDOWTEST

#include <memory>

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
        editorWindow = std::unique_ptr<EditorWindow>(new EditorWindow(SettingsBackend::getSettings(0)));
    }
    void objectCreationTest() {
        QVERIFY(editorWindow.get());
    }
private:
    std::unique_ptr<EditorWindow> editorWindow;
};
#endif // EDITORWINDOWTEST
