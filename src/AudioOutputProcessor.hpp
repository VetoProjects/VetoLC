#ifndef AUDIOOUTPUTPROCESSOR_HPP
#define AUDIOOUTPUTPROCESSOR_HPP

#include <QTimer>

class AudioOutputProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutputProcessor(QObject *parent = 0);
    ~AudioOutputProcessor();

public:
    void write(const char *data, qint64 len);

signals:
    void stopWriting();
    void startWriting();

private:
    quint64 currentPlaying, currentWriting, currentLen;
    char **dataBuffer;
    qint64 *lenBuffer;
    QTimer *timer;

private slots:
    void writeToDevice();

};

#endif // AUDIOOUTPUTPROCESSOR_HPP
