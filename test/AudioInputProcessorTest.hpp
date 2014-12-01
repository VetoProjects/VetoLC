#ifndef AUDIOINPUTPROCESSORTEST
#define AUDIOINPUTPROCESSORTEST

#include <memory>

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
        processor = std::unique_ptr<AudioInputProcessor>(new AudioInputProcessor());
    }
    void objectCreationTest(){
        QVERIFY(processor.get());
    }
    void formatTest(){
        QVERIFY(processor->format().isValid());
    }
private:
    std::unique_ptr<AudioInputProcessor> processor;
};

#endif // AUDIOINPUTPROCESSORTEST
