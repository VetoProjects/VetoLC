#include "AudioInputProcessor.hpp"

AudioInputProcessor::AudioInputProcessor(QObject *parent) :
    QIODevice(parent)
{
    auto inputDevice = QAudioDeviceInfo::defaultInputDevice();
    for(const auto dev : QAudioDeviceInfo::availableDevices(QAudio::AudioInput)){
        if(dev.deviceName().contains("output", Qt::CaseInsensitive)){
            inputDevice = dev;
            if(dev.deviceName().contains("analog", Qt::CaseInsensitive))
                break;
        }
    }

    QAudioFormat format;
//    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setChannelCount(2);
    format.setCodec("audio/pcm");
    format.setSampleRate(8000);
    format.setSampleSize(32);
    format.setSampleType(QAudioFormat::Float);

    if(!inputDevice.isFormatSupported(format)){
        qWarning() << tr("Format is not supported");
        format = inputDevice.nearestFormat(format);
        qDebug() << tr("\tchannels:") << format.channelCount();
        qDebug() << tr("\tsample rate:") << format.sampleRate();
        qDebug() << tr("\tsample size:") << format.sampleSize();
        qDebug() << tr("\tsample type:") << format.sampleType();
    }

    input = std::unique_ptr<QAudioInput>(new QAudioInput(inputDevice, format, this));
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
//    int bufSize = input->bufferSize() / 5;
//    if(len < bufSize)
//        return 0;
    Q_EMIT processData(QByteArray(data, len));
    return len;
}
