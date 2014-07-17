#ifndef RENDERERTEST
#define RENDERERTEST

#include <QObject>
#include <QTest>
#include "../src/LiveThread.hpp"

class RendererTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        thread = new GlLiveThread(0);
        connect(thread, SIGNAL(doneSignal(GlLiveThread*, QString)),
                this, SLOT(finishedTest(GlLiveThread*, QString)));
        thread->initialize("Test", "This is not valid code;");
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedTest(GlLiveThread* returnedThread, QString returned){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned != NULL);
    }
    void cleanupTestCase(){
        delete thread;
    }

private:
    LiveThread* thread;
};

#endif // RENDERERTEST
