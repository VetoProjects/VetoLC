#ifndef PYLIVETEST_HPP
#define PYLIVETEST_HPP
#ifdef WITH_PYTHON

#include <QObject>
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
    void initTestCase(){
        thread = new PyLiveThread(0);
        connect(thread, SIGNAL(doneSignal(PyLiveThread*, QString, int)),
                this, SLOT(finishedTest(PyLiveThread*, QString, int)));
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
    void finishedTest(PyLiveThread* returnedThread, QString returned, int lineno){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned != NULL);
        QVERIFY(lineno == 1);
        QVERIFY(thread == returnedThread);
    }
    void cleanupTestCase(){
        delete thread;
    }

private:
    LiveThread* thread;
};
#endif
#endif // PYLIVETEST_HPP
