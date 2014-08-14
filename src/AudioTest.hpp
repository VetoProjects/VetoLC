#ifndef AUDIOTEST_HPP
#define AUDIOTEST_HPP

#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include <cmath>
#include "AudioOutputProcessor.hpp"

#define PI (3.1415926535897932384626433832795)

/**
 * @brief The AudioTest class
 *
 * Test thread for direct audio output with AudioOutputProcessor.
 * Generates a 440 Hz sin-wave-ton.
 */
class AudioTest : public QThread{
    Q_OBJECT
public:
    AudioTest(QObject *parent = 0) : QThread(parent){
    }

    virtual ~AudioTest(){
        delete mtx;
    }

    virtual void run() Q_DECL_OVERRIDE {
        mtx = new QMutex();
        aop = new AudioOutputProcessor();
        connect(aop, SIGNAL(startWriting()), this, SLOT(startWriting()));
        cycle = 0;
        write();
    }

    void write(){
//        qDebug("start writing.");
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

#endif // AUDIOTEST_HPP
