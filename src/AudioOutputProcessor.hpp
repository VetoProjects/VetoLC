#ifndef AUDIOOUTPUTPROCESSOR_HPP
#define AUDIOOUTPUTPROCESSOR_HPP

#include <QThread>
#include <QAudioOutput>
#include <QIODevice>

class AudioOutputProcessor : public QThread
{
    Q_OBJECT
public:
    explicit AudioOutputProcessor(QObject *parent = 0);
    ~AudioOutputProcessor();

    bool write(const char *data, qint64 len);

signals:
    void stopWriting();
    void startWriting();

private:
    int currentPlaying, currentWriting;
    char **dataBuffer;
    qint64 *lenBuffer;
    QAudioOutput *audioOut;
    QIODevice *device;

private slots:
    virtual void run() Q_DECL_OVERRIDE;

};

#endif // AUDIOOUTPUTPROCESSOR_HPP
