#ifndef CODEHIGHLIGHTERTEST
#define CODEHIGHLIGHTERTEST

#include <QObject>
#include <QTest>
#include "../CodeHighlighter.hpp"

class CodeHighlighterTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        codeHighlighter = new CodeHighlighter();
    }
    void objectCreationTest() {
        QVERIFY(codeHighlighter);
    }
    void cleanupTestCase() {
        delete codeHighlighter;
    }
private:
    CodeHighlighter *codeHighlighter;
};

#endif // CODEHIGHLIGHTERTEST
