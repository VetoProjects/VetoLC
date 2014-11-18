#ifndef AUDIOOUTPUTPROCESSOR_HPP
#define AUDIOOUTPUTPROCESSOR_HPP

#include <QThread>
#include <QAudioOutput>
#include <QIODevice>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>


class AudioOutputProcessor : public QThread
{
    Q_OBJECT
public:
    explicit AudioOutputProcessor(QObject *parent = 0);
    ~AudioOutputProcessor();

    bool write(const char *data, qint64 len);

Q_SIGNALS:
    void stopWriting();
    void startWriting();
    void postWriteToDevice();

private:
    int currentPlaying, currentWriting;
    char **dataBuffer;
    qint64 *lenBuffer;
    QAudioOutput *audioOut;
    QIODevice *device;
    bool queued;
    qint64 currentLen;

private Q_SLOTS:
    virtual void run() Q_DECL_OVERRIDE;
    void writeToDevice();

};

#endif // AUDIOOUTPUTPROCESSOR_HPP
