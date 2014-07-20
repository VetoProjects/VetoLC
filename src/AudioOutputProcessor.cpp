#include "AudioOutputProcessor.hpp"


AudioOutputProcessor::AudioOutputProcessor(QObject *parent) : QObject(parent),
    currentPlaying(0), currentWriting(0), currentLen(0),
    dataBuffer(new char*[4]), lenBuffer(new qint64[4]),
    timer(new QTimer(this))
{
    for(int i = 0; i < 4; ++i)
        dataBuffer[i] = 0;
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeToDevice()));
}

AudioOutputProcessor::~AudioOutputProcessor()
{
    for(int i = 0; i < 4; ++i)
        delete dataBuffer[i];
    delete dataBuffer;
    delete lenBuffer;
    delete timer;
}


void AudioOutputProcessor::write(const char *data, qint64 len)
{
    lenBuffer[currentWriting] = len;
    dataBuffer[currentWriting] = new char[len];
    memcpy(dataBuffer[currentWriting], data, (size_t) len);
    currentWriting = (currentWriting + 1) % 4;
    if(!timer->isActive()){
        currentLen = len;
        timer->start();
    }
}


void AudioOutputProcessor::writeToDevice()
{
    if(currentLen == 0){
        delete dataBuffer[currentPlaying];
        currentPlaying = (currentPlaying + 1) % 4;
        if(currentPlaying == currentWriting)
            timer->stop();
        else
            currentLen = lenBuffer[currentPlaying];
    }
}

