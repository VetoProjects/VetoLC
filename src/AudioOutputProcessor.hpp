#ifndef AUDIOOUTPUTPROCESSOR_HPP
#define AUDIOOUTPUTPROCESSOR_HPP

#include <QIODevice>
#include <QAudioFormat>

class AudioOutputProcessor : public QIODevice
{
public:
    AudioOutputProcessor(QAudioFormat audioFormat, QObject *parent = 0);
    ~AudioOutputProcessor();

    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    QAudioFormat format;
    char *buffer;
    int offset, count;
};

#endif // AUDIOOUTPUTPROCESSOR_HPP
