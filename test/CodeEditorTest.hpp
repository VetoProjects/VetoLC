#ifndef CODEEDITORTEST
#define CODEEDITORTEST

#include <QObject>
#include <QTest>
#include "../src/CodeEditor.hpp"

/**
 * @brief The CodeEditor Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the CodeEditor class; functionality tested includes
 * object creation and writing to the text editor.
 */
class CodeEditorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        codeEditor = new CodeEditor();
    }
    void objectCreationTest(){
        QVERIFY(codeEditor);
    }
    void writingTest(){
        QTest::keyClicks(codeEditor, "I am an int and that is code;");
        QCOMPARE(codeEditor->toPlainText(), QStringLiteral("I am an int and that is code;"));
    }
    void cleanupTestCase() {
        delete codeEditor;
    }
private:
    CodeEditor *codeEditor;
};
#endif // CODEEDITORTEST
