#include "AudioInputProcessor.hpp"

AudioInputProcessor::AudioInputProcessor(QObject *parent) :
    QIODevice(parent)
{
    QAudioFormat format;
//    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleRate(8000);
    format.setSampleSize(32);
    format.setSampleType(QAudioFormat::Float);
    if(!QAudioDeviceInfo::defaultInputDevice().isFormatSupported(format)){
        qWarning(tr("Format is not supported"));
        format = QAudioDeviceInfo::defaultInputDevice().nearestFormat(format);
        qDebug(tr("\tchannels: %d"), format.channelCount());
        qDebug(tr("\tsample rate: %d"), format.sampleRate());
        qDebug(tr("\tsample size: %d"), format.sampleSize());
        qDebug(tr("\tsample type: %d"), format.sampleType());
    }

    input = new QAudioInput(format, this);
}

void AudioInputProcessor::start()
{
    this->open(QIODevice::WriteOnly);
    input->start(this);
}

const QAudioFormat AudioInputProcessor::format() const
{
    return input->format();
}

qint64 AudioInputProcessor::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    Q_UNUSED(maxlen);
    return 0;
}

qint64 AudioInputProcessor::writeData(const char *data, qint64 len)
{
    int bufSize = input->bufferSize() / 5;
    if(len < bufSize)
        return 0;
    emit processData(QByteArray(data + len - bufSize, bufSize));
    return len;
}
