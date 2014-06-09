#ifndef PYSOUNDGENERATORTEST
#define PYSOUNDGENERATORTEST
#ifdef with_python
#include <QObject>
#include <QTest>
#include "../src/PySoundGenerator.hpp"

class PySoundGeneratorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase() {
        pySoundGenerator = new PySoundGenerator((char*)"Test", (char*)"raise ImportError(\"Not valid\")");
        connect(pySoundGenerator, SIGNAL(doneSignal(PythonException)), this, SLOT(finishedTest(PythonException)));
    }
    void objectCreationTest() {
        QVERIFY(pySoundGenerator);
    }
    void runTest(){
        pySoundGenerator->start();
        QTest::qWait(200);
    }
    void finishedTest(PythonException returned){
        QVERIFY(returned.what() == NULL);
    }
    void cleanupTestCase() {
        delete pySoundGenerator;
    }
private:
    PySoundGenerator *pySoundGenerator;
};
#endif
#endif // PYSOUNDGENERATORTEST
