#ifndef AUDIOPROCESSORINSTANCE
#define AUDIOPROCESSORINSTANCE

#include <QThread>
#include <QMutex>
#include <cmath>
#include <QDebug>
#include "AudioOutputProcessor.hpp"

#define PI (3.1415926535897932384626433832795)


class AudioProcessorInstance : public QThread{
    Q_OBJECT
public:
    AudioProcessorInstance(QObject *parent = 0) : QThread(parent){
    }

    virtual void run() Q_DECL_OVERRIDE {
        cycle = 0;
        aop = new AudioOutputProcessor();
        connect(this, SIGNAL(startWriting()), this, SLOT(write()), Qt::QueuedConnection);
        connect(aop, SIGNAL(startWriting()), this, SLOT(write()), Qt::QueuedConnection);
        connect(aop, SIGNAL(started()), this, SLOT(write()), Qt::QueuedConnection);
        aop->start();
        exec();
    }

Q_SIGNALS:
    void startWriting();

public Q_SLOTS:
    void write(){
        qDebug() << "write 1024 stereo samples."; // not working without!?
        short buffer[2048];
        for(int i = 0; i < 1024; ++i)
            buffer[i * 2] = buffer[i * 2 + 1] = (short)(generate((double)(i + cycle) / 96000.0) * 16000.0);
        cycle += 1024;
        if(aop->write((char*)buffer, 4096))
            Q_EMIT startWriting();
        //qDebug() << "stop writing.";
    }

private:
    AudioOutputProcessor *aop;
    int cycle;

    double generate(double t){
        double sin440 = sin(2.0 * PI * t * 440.0);
//        double sin445 = sin(2.0 * PI * t * 445.0);
        return sin440;
    }
};

#endif
