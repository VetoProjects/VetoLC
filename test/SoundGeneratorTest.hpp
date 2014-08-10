#ifndef SOUNDGENERATORTEST
#define SOUNDGENERATORTEST

#include <QObject>
#include <QTest>
#include "../src/LiveThread.hpp"

/**
 * @brief The SoundGeneratorTest class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the PySoundGenerator class; functionality tested
 * includes object creation, thread management, code
 * execution and return validity.
 */
class SoundGeneratorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        thread = new QtSoundThread(0);
        connect(thread, SIGNAL(doneSignal(QtSoundThread*, QString)),
                this, SLOT(finishedTest(QtSoundThread*, QString)));
        thread->initialize((char *)"", (char *)"int i;");
    }
    void objectCreationTest() {
        QVERIFY(thread);
    }
    void runTest(){
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedTest(QtSoundThread* returnedThread, QString returned){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned == QString("Terminated successfully."));
    }
    void cleanupTestCase(){
        delete soundGenerator;
        delete thread;
    }

private:
    SoundGenerator* soundGenerator;
    LiveThread* thread;
};
#endif // SOUNDGENERATORTEST
