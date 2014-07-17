#include "AudioOutputProcessor.hpp"

AudioOutputProcessor::AudioOutputProcessor(QAudioFormat audioFormat, QObject *parent) :
    QIODevice(parent),
    format(audioFormat), offset(0), count(0)
{
//    buffer = (char*)malloc(audioFormat.bytesPerFrame() * 1024); // reserve 1024 frames
    buffer = (char*)malloc(8192 * 10); // reserve 10 chunks of python sound output (to much?)
}

AudioOutputProcessor::~AudioOutputProcessor()
{
    delete buffer;
}

qint64 AudioOutputProcessor::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioOutputProcessor::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
//    memcpy(buffer + offset + count, data, max)
}
