#ifndef PYLIVETEST_HPP
#define PYLIVETEST_HPP

#include <QTest>

#include "../src/LiveThread.hpp"

/**
 * @brief The PyLiveThreadTest class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the PyLiveThread class; functionality tested
 * includes object creation, thread management, code
 * execution and return validity.
 */
class PyLiveTest : public QObject{
Q_OBJECT
private slots:
    void init(){
        thread = new PyLiveThread(0);
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        connect(thread, SIGNAL(doneSignal(PyLiveThread*, QString, int)),
                this, SLOT(finishedRunTest(PyLiveThread*, QString, int)));
        thread->initialize("Test", "raise ImportError('Not valid')");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedRunTest(PyLiveThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral("<type 'exceptions.ImportError'>: 'Not valid' at line 1"));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, 1);
    }
    void commandTest(){
        connect(thread, SIGNAL(doneSignal(PyLiveThread*, QString, int)),
                this, SLOT(finishedCommandTest(PyLiveThread*, QString, int)));
        thread->initialize("Test", "\t0 >= 1");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedCommandTest(PyLiveThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral("<type 'exceptions.IndentationError'>: 'unexpected indent' at line 1"));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, 1);
    }
    void validTest(){
        connect(thread, SIGNAL(doneSignal(PyLiveThread*, QString, int)),
                this, SLOT(finishedValidTest(PyLiveThread*, QString, int)));
        thread->initialize("Test", "for i in range(100): print(i)");
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedValidTest(PyLiveThread* returnedThread, QString returned, int lineno){
        QCOMPARE(returnedThread, thread);
        QCOMPARE(returned, QStringLiteral("Script terminated successfully."));
        QCOMPARE(thread, returnedThread);
        QCOMPARE(lineno, -1);
    }

private:
    LiveThread* thread;
};
#endif // PYLIVETEST_HPP
