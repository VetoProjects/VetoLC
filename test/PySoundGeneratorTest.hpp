#ifndef PYSOUNDGENERATORTEST
#define PYSOUNDGENERATORTEST

#include <QObject>
#include <QTest>
#include <QDebug>
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
    void init(){
        thread = new PySoundThread(0);
        }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        connect(thread, SIGNAL(doneSignal(PySoundThread*, QString, int)),
                this, SLOT(finishedRunTest(PySoundThread*, QString, int)));
        thread->initialize("Test", "raise ImportError('Not valid')");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedRunTest(PySoundThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral("<type 'exceptions.ImportError'>: 'Not valid' at line 1"));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, 1);
    }
    void commandTest(){
        connect(thread, SIGNAL(doneSignal(PySoundThread*, QString, int)),
                this, SLOT(finishedCommandTest(PySoundThread*, QString, int)));
        thread->initialize("Test", "\t0 >= 1");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedCommandTest(PySoundThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral("<type 'exceptions.IndentationError'>: 'unexpected indent' at line 1"));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, 1);
    }
    /*void validTest(){
        connect(thread, SIGNAL(doneSignal(PySoundThread*, QString, int)),
                this, SLOT(finishedValidTest(PySoundThread*, QString, int)));
        thread->initialize("Test", "channels = ()");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedValidTest(PySoundThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral(""));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, 0);
    }*/
    void cleanup(){
        while(thread->isRunning())
            ;
        delete thread;
    }

private:
    LiveThread* thread;
};
#endif // PYSOUNDGENERATORTEST
