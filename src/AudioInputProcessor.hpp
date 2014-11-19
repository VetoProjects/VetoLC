#ifndef AUDIOINPUTPROCESSOR_HPP
#define AUDIOINPUTPROCESSOR_HPP

#include <QIODevice>
#include <QDebug>
#include <QAudioInput>
#include <QMutex>
#include <QByteArray>
#include <QAudioBuffer>

class AudioInputProcessor : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioInputProcessor(QObject *parent = 0);
    void start();
    const QAudioFormat format() const;

Q_SIGNALS:
    void processData(QByteArray);


private:
    std::unique_ptr<QAudioInput> input;
//    QMutex dataAccess;
//    char *data;


    // QIODevice interface
protected:
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);
};

#endif // AUDIOINPUTPROCESSOR_HPP
