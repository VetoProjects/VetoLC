#ifndef PYSOUNDGENERATORTEST
#define PYSOUNDGENERATORTEST
#ifdef with_python

#include <QObject>
#include <QTest>
#include "../src/LiveThread.hpp"

/**
 * @brief The PySoundGeneratorTest class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the PySoundGenerator class; functionality tested
 * includes object creation, thread management, code
 * execution and return validity.
 */
class PySoundGeneratorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        thread = new PySoundThread(0);
        connect(thread, SIGNAL(doneSignal(PySoundThread*, QString)),
                this, SLOT(finishedTest(PySoundThread*, QString)));
        thread->initialize("Test", "raise ImportError('Not valid')");
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedTest(PySoundThread* returnedThread, QString returned){
        QVERIFY(returnedThread == thread);
        qDebug() << returned;
        QVERIFY(returned != NULL);
        QVERIFY(thread == returnedThread);
    }
    void cleanupTestCase(){
        delete thread;
    }

private:
    LiveThread* thread;
};
#endif
#endif // PYSOUNDGENERATORTEST
