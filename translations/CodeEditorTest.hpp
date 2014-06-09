#ifndef CODEEDITORTEST
#define CODEEDITORTEST

#include <QObject>
#include <QtGui>
#include <QtTest/QtTest>
#include "CodeEditor.hpp"

/**
 * @brief The SetupTest Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 * @version 0.1.0
 */
class CodeEditorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        codeEditor = new CodeEditor();
    }
    void objectCreationTest() {
        QVERIFY(codeEditor);
    }
    void writeTest() {
        QTest::keyClicks(&codeEditor, "I am an int and that is code;");
        QCOMPARE(codeEditor.text(), "I am an int and that is code;");
    }
    void cleanupTestCase() {
        delete codeEditor;
    }
private:
    CodeEditor *codeEditor;
};
QTEST_MAIN(TestTest)
#include "testtest.moc"
#endif // CODEEDITORTEST
