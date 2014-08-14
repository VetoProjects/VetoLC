#ifndef AUDIOOUTPUTPROCESSORTEST
#define AUDIOOUTPUTPROCESSORTEST

#include <QTest>
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <cmath>
#include "../src/AudioOutputProcessor.hpp"

#define PI (3.1415926535897932384626433832795)

/**
 * @brief The AudioProcessorInstance class
 *
 * A basic implementation of a sound processor using our output.
 * This plays a sine wave at 440Hz.
 */
class AudioProcessorInstance : public QThread{
    Q_OBJECT
public:
    AudioProcessorInstance(QObject *parent = 0) : QThread(parent){
    }

    virtual ~AudioProcessorInstance(){
        aop->terminate();
        delete aop;
    }

    virtual void run() Q_DECL_OVERRIDE {
        mtx = new QMutex();
        aop = new AudioOutputProcessor();
        connect(aop, SIGNAL(startWriting()), this, SLOT(startWriting()));
        cycle = 0;
        write();
    }

    void write(){
        short buffer[2048];
        do{
            for(int i = 0; i < 1024; ++i)
                buffer[i * 2] = buffer[i * 2 + 1] = (int)(generate((double)(i + cycle) / 96000.0) * 16000.0);
            cycle += 1024;
        }while(aop->write((char*)buffer, 4096));
    }

public slots:
    void startWriting(){ write(); }

private:
    AudioOutputProcessor *aop;
    QMutex *mtx;
    int cycle;

    double generate(double t){
        double sin440 = sin(2.0 * PI * t * 440.0);
//        double sin445 = sin(2.0 * PI * t * 445.0);
        return sin440;
    }
};

/**
 * @brief The AudioOutputProcessorTest class
 *
 * Tests the AudiOutputProcessor class; functionality
 * tested includes object creation and writing to the
 * audio output.
 */
class AudioOutputProcessorTest : public QObject{
Q_OBJECT
private slots:
    void initTestCase(){
        api = new AudioProcessorInstance();
    }
    void objectCreationTest(){
        QVERIFY(api);
    }
    void writeTest(){
        api->start();
        QTest::qWait(1000);
        api->terminate();
    }
    void cleanupTestCase(){
        delete api;
    }

private:
    AudioProcessorInstance *api;
};
#endif // AUDIOOUTPUTPROCESSORTEST
