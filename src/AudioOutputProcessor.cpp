#include "AudioOutputProcessor.hpp"

AudioOutputProcessor::AudioOutputProcessor(QObject *parent) : QThread(parent),
    dataBuffer(0), lenBuffer(0), audioOut(0), device(0)
{ }

AudioOutputProcessor::~AudioOutputProcessor()
{
    if(audioOut){
        audioOut->stop();
        delete audioOut;
    }

    if(dataBuffer){
        for(int i = 0; i < 4; ++i)
            delete dataBuffer[i];
        delete[] dataBuffer;
    }
    delete[] lenBuffer;
}

void AudioOutputProcessor::run()
{
    currentLen = 0;
    currentWriting = 0;
    currentPlaying = 0;

    dataBuffer = new char*[4];
    lenBuffer = new qint64[4];

    for(int i = 0; i < 4; ++i){
        dataBuffer[i] = 0;
        lenBuffer[i] = 0;
    }

    QAudioFormat format;
    format.setSampleRate(96000); // Usually this is specified through an UI option
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << tr("raw audio format not supported by backend, cannot play audio.");
        return;
    }

    audioOut = new QAudioOutput(format);
    device = audioOut->start();

    queued = false;
    connect(this, SIGNAL(postWriteToDevice()), this, SLOT(writeToDevice()), Qt::QueuedConnection);

    exec();
}


bool AudioOutputProcessor::write(const char *data, qint64 len)
{
//    qDebug() << "write to buffer";
//    qDebug() << "buffering" << currentPlaying << "/" << currentWriting;
    if(currentPlaying == currentWriting && currentLen > 0){
        qWarning() << tr("failed to write: buffer is full");
        return false;
    }

    lenBuffer[currentWriting] = len;
    dataBuffer[currentWriting] = new char[len];
    memcpy(dataBuffer[currentWriting], data, (size_t) len);

    currentWriting = (currentWriting + 1) % 4;

    auto full = (currentPlaying == currentWriting);

    if(full)
        Q_EMIT stopWriting();

    if(!queued){
        queued = true;
        currentLen = lenBuffer[currentPlaying];
        Q_EMIT postWriteToDevice();
//        qDebug() << "write to device posted from buffering";
    }

    return !full;
}

void AudioOutputProcessor::writeToDevice()
{
//    qDebug() << "write to device. left:" << currentLen;
    queued = false;
    currentLen -= device->write(dataBuffer[currentPlaying] + lenBuffer[currentPlaying] - currentLen, currentLen);

    if(currentLen <= 0){
        lenBuffer[currentPlaying] = 0;
        delete dataBuffer[currentPlaying];
        if(currentPlaying == currentWriting){
            currentPlaying = (currentPlaying + 1) % 4;
            Q_EMIT startWriting();
        }else
            currentPlaying = (currentPlaying + 1) % 4;
        currentLen = lenBuffer[currentPlaying];
    }

    if(currentLen > 0 && !queued){
        queued = true;
        Q_EMIT postWriteToDevice();
//        qDebug() << "write to device posted from writing. left:" << currentLen;
    }
}
