#ifndef CODEHIGHLIGHTERTEST_H
#define CODEHIGHLIGHTERTEST_H

#include <QObject>
#include <QTest>
#include "../src/CodeHighlighter.hpp"

/**
 * @brief The CodeHighlighter Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the CodeHighlighter class; functionality tested includes
 * object creation and updating highlighting.
 */
class CodeHighlighterTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        codeHighlighter = new CodeHighlighter();
    }
    void objectCreationTest(){
        QVERIFY(codeHighlighter);
    }
    void updateTest(){
        QVERIFY(codeHighlighter->setupHighlighting(1));
        QVERIFY(codeHighlighter->setupHighlighting(1000) == false);
        QVERIFY(codeHighlighter->setupHighlighting(-10) == false);
    }
    void cleanupTestCase() {
        delete codeHighlighter;
    }
private:
    CodeHighlighter *codeHighlighter;
};

#endif // CODEHIGHLIGHTERTEST_H
