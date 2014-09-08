#ifndef RENDERERTEST
#define RENDERERTEST

#include <QObject>
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
    void initTestCase(){
        thread = new GlLiveThread(0);
        connect(thread, SIGNAL(errorSignal(QString, int)),
                this, SLOT(finishedTest(QString, int)));
        connect(thread, SIGNAL(doneSignal(GlLiveThread*, QString)),
                 this, SLOT(doneTest(GlLiveThread*, QString)));
        thread->initialize("Test", "This is not valid code;");
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        thread->start();
        QTest::qWait(1000);
        thread->terminate();
    }
    void finishedTest(QString returned, int lineno){
        QVERIFY(returned != NULL);
        QVERIFY(returned == "ERROR: 0:4: 'This' : syntax error syntax error");
        QVERIFY(lineno == 1);
    }
    void doneTest(GlLiveThread* returnedThread, QString err){
        QVERIFY(returnedThread == thread);
    }
    void cleanupTestCase(){
        delete thread;
    }

private:
    LiveThread* thread;
};

#endif // RENDERERTEST
