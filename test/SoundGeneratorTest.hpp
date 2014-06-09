#ifndef SOUNDGENERATORTEST
#define SOUNDGENERATORTEST

#include <QObject>
#include <QTest>
#include "../src/LiveThread.hpp"

class SoundGeneratorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        thread = new QtSoundThread(0);
        connect(thread, SIGNAL(doneSignal(LiveThread*, QtSoundException)),
                this, SLOT(finishedTest(LiveThread*, QtSoundException)));
        thread->initialize((char *)"", (char *)"int i;");
    }
    void objectCreationTest() {

        QVERIFY(thread);
    }
    void runTest(){
        thread->start();
        QTest::qWait(200);
        thread->terminate();
    }
    void finishedTest(LiveThread* returnedThread, QtSoundException returned){
        QVERIFY(returnedThread == thread);
        QVERIFY(returned.what() == QString("Terminated successfully."));
        QVERIFY(thread == returnedThread);
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
