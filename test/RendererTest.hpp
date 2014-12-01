#ifndef RENDERERTEST
#define RENDERERTEST

#include <QTest>

#include "../src/LiveThread.hpp"

/**
 * @brief The RendererTest class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the Renderer class; functionality tested
 * includes object creation, thread management, code
 * execution and return validity.
 */
class RendererTest : public QObject{
Q_OBJECT
private slots:
    void init(){
        thread = new GlLiveThread(0);
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        connect(thread, SIGNAL(errorSignal(GlLiveThread*, QString, int)),
                this, SLOT(finishedTest(GlLiveThread*, QString, int)));
        connect(thread, SIGNAL(doneSignal(GlLiveThread*, QString)),
                 this, SLOT(doneTest(GlLiveThread*, QString)));
        thread->initialize("Test", "This is not valid code;");
        thread->start();
        QTest::qWait(1000);
        thread->terminate();
    }
    void commandTest(){
        connect(thread, SIGNAL(errorSignal(GlLiveThread*, QString, int)),
                this, SLOT(finishedCommandTest(GlLiveThread*, QString, int)));
        connect(thread, SIGNAL(doneSignal(GlLiveThread*, QString)),
                 this, SLOT(doneTest(GlLiveThread*, QString)));
        thread->initialize("Test", "#version 330 core");
        thread->start();
        QTest::qWait(1000);
        thread->terminate();
    }
    void finishedTest(GlLiveThread* returnedThread, QString returned, int lineno){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned != "");
        QCOMPARE(returned, QStringLiteral("ERROR: 0:1: '' :  #version required and missing.\nERROR: 0:4: 'This' : syntax error: syntax error\n"));
        QVERIFY(lineno == 1);
    }
    void finishedCommandTest(GlLiveThread* returnedThread, QString returned, int lineno){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned != "");
        QCOMPARE(returned, QStringLiteral("ERROR: Compiled fragment shader was corrupt."));
        QCOMPARE(0, 1);
        QVERIFY(lineno == 0);
    }
    void doneTest(GlLiveThread* returnedThread, QString err){
        QVERIFY(returnedThread == thread);
        Q_UNUSED(err);
    }
    void cleanup(){
        delete thread;
    }

private:
    LiveThread* thread;
};

#endif // RENDERERTEST
