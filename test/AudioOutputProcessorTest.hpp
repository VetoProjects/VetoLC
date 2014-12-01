#ifndef AUDIOOUTPUTPROCESSORTEST
#define AUDIOOUTPUTPROCESSORTEST

#include <memory>
#include <cmath>

#include <QTest>

#include "../src/AudioOutputProcessor.hpp"

#define PI (3.1415926535897932384626433832795)

//class AudioProcessorInstance : public QThread{
//Q_OBJECT
//public:
//    AudioProcessorInstance(QObject *parent = 0) : QThread(parent){
//        mtx = new QMutex();
//        aop = new AudioOutputProcessor();
//        connect(aop, SIGNAL(startWriting()), this, SLOT(startWriting()));
//        cycle = 0;
//    }
//    virtual ~AudioProcessorInstance(){
//        delete mtx;
//    }
//    virtual void run()Q_DECL_OVERRIDE {
//        while(true){
//            //        qDebug("start writing.");
//            if(breakSignal == true)
//                return;
//            short buffer[2048];
//            for(int i = 0; i < 1024; ++i)
//                buffer[i * 2] = buffer[i * 2 + 1] = (int)(generate((double)(i + cycle) / 96000.0) * 16000.0);
//            cycle += 1024;
//            if(!aop->write((char*)buffer, 4096))
//                breakSignal = true;
//        }
//    }
//public slots:
//    void startWriting(){breakSignal = false; }
//    void stopWriting(){breakSignal = true;}

//private:
//    AudioOutputProcessor *aop;
//    QMutex *mtx;
//    int cycle;
//    bool breakSignal;
//    double generate(double t){
//        double sin440 = sin(2.0 * PI * t * 440.0);
//        //        double sin445 = sin(2.0 * PI * t * 445.0);
//        return sin440;
//    }
//};


class AudioProcessorInstance : public QThread{
    Q_OBJECT
public:
    AudioProcessorInstance(QObject *parent = 0) : QThread(parent){
    }

    ~AudioProcessorInstance(){
        aop->terminate();
        while(aop->isRunning())
            ;
    }

    virtual void run() Q_DECL_OVERRIDE {
        cycle = 0;
        aop = std::unique_ptr<AudioOutputProcessor>(new AudioOutputProcessor());
        connect(this, SIGNAL(startWriting()), this, SLOT(write()), Qt::QueuedConnection);
        connect(aop.get(), SIGNAL(startWriting()), this, SLOT(write()), Qt::QueuedConnection);
        connect(aop.get(), SIGNAL(started()), this, SLOT(write()), Qt::QueuedConnection);
        aop->start();
        exec();
    }

signals:
    void startWriting();

public slots:
    void write(){
        qDebug() << "write 1024 stereo samples."; // not working without!?
        short buffer[2048];
        for(int i = 0; i < 1024; ++i)
            buffer[i * 2] = buffer[i * 2 + 1] = (short)(generate((double)(i + cycle) / 96000.0) * 16000.0);
        cycle += 1024;
        if(aop->write((char*)buffer, 4096))
            emit startWriting();
        //qDebug() << "stop writing.";
    }

private:
    std::unique_ptr<AudioOutputProcessor> aop;
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
        api = std::unique_ptr<AudioProcessorInstance>(new AudioProcessorInstance());
    }
    void objectCreationTest(){
        QVERIFY(api.get());
    }
    void writeTest(){
        api->start();
        QTest::qWait(1000);
        api->terminate();
        while(api->isRunning())
            ;
        QTest::qWait(15);
     }

private:
    std::unique_ptr<AudioProcessorInstance> api;
};
#endif // AUDIOOUTPUTPROCESSORTEST
