#ifndef CODEEDITORTEST
#define CODEEDITORTEST

#include <memory>

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
        codeEditor = std::unique_ptr<CodeEditor>(new CodeEditor());
    }
    void objectCreationTest(){
        QVERIFY(codeEditor.get());
    }
    void writingTest(){
        QTest::keyClicks(codeEditor.get(), "I am an int and that is code;");
        QCOMPARE(codeEditor->toPlainText(), QStringLiteral("I am an int and that is code;"));
    }
    void LineHighlightingTest(){
        QVERIFY(codeEditor->lineHighlightingWidth() == 10);
    }
private:
    std::unique_ptr<CodeEditor> codeEditor;
};
#endif // CODEEDITORTEST
