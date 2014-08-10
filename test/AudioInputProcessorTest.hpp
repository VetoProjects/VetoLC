#ifndef AUDIOINPUTPROCESSORTEST
#define AUDIOINPUTPROCESSORTEST

#include <QObject>
#include <QTest>
#include "../src/AudioInputProcessor.hpp"

/**
 * @brief The AudioInputProcessor Testing class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539501)
 *
 * Tests the Backend class; functionality tested includes
 * object creation, instance creation and removing and
 * settings management.
 */
class AudioInputProcessorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        processor = new AudioInputProcessor();
    }
    void objectCreationTest(){
        QVERIFY(processor);
    }
    void formatTest(){
        QVERIFY(processor->format().isValid());
    }
    void cleanupTestCase(){
        delete processor;
    }
private:
    AudioInputProcessor *processor;
};

#endif // AUDIOINPUTPROCESSORTEST
