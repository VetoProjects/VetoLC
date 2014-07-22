#include "AudioOutputProcessor.hpp"
#include <QThread>
#include <QDebug>


AudioOutputProcessor::AudioOutputProcessor(QObject *parent) : QThread(parent),
    currentPlaying(0), currentWriting(0),
    dataBuffer(new char*[4]), lenBuffer(new qint64[4])
{
    for(int i = 0; i < 4; ++i)
        dataBuffer[i] = 0;

    QAudioFormat format;
    format.setSampleRate(96000); // Usually this is specified through an UI option
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning(tr("raw audio format not supported by backend, cannot play audio."));
        return;
    }

    audioOut = new QAudioOutput(format, this);
    device = audioOut->start();
}

AudioOutputProcessor::~AudioOutputProcessor()
{
    quit();
    audioOut->stop();

    for(int i = 0; i < 4; ++i)
        delete dataBuffer[i];
    delete dataBuffer;
    delete lenBuffer;
    delete audioOut;
}


bool AudioOutputProcessor::write(const char *data, qint64 len)
{
    if(isRunning() && currentPlaying == currentWriting){
        qDebug(tr("failed to write: buffer is full"));
        return false;
    }

    lenBuffer[currentWriting] = len;
    delete dataBuffer[currentWriting];
    dataBuffer[currentWriting] = new char[len];
    memcpy(dataBuffer[currentWriting], data, (size_t) len);

    currentWriting = (currentWriting + 1) % 4;
    if(!isRunning()){
        start();
    }else if(currentPlaying == currentWriting){
        emit stopWriting();
        return false;
    }
    return true;
}


void AudioOutputProcessor::run()
{
    do{
        qint64 currentLen = lenBuffer[currentPlaying];
        while(currentLen > 0){
            currentLen -= device->write(dataBuffer[currentPlaying] + lenBuffer[currentPlaying] - currentLen, currentLen);
            usleep(10);
        }
        bool start = currentPlaying == currentWriting;
        currentPlaying = (currentPlaying + 1) % 4;
        if(start)
            emit startWriting();
    }while(currentPlaying != currentWriting);
    quit();
}
