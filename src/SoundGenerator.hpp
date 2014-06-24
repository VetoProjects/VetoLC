#ifndef SOUNDGENERATOR
#define SOUNDGENERATOR

#include <exception>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioFormat>
//#include <QtQml/QQmlEngine>
#include <QList>
#include <QFile>
#include <QPair>
#include <QException>
#include <QThread>
#include <QAction>
#include "QtSoundException.hpp"

/**
 * @brief The SoundGenerator class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of QThread that implements an environment for sound
 * live coding while keeping the main codeeditor thread clean by
 * dispatching. QT-based.
 */
class SoundGenerator : public QObject{
Q_OBJECT

public:
    SoundGenerator(const QString &, const QString &);
    //~SoundGenerator(){}
    void run();
    bool updateCode(const QString &, const QString &);

private:
    int addToOutputs(QFile, QAudioFormat);
    void removeFromOutputs(int);
    void changeOutputAt(int, QFile*);
    void loop();
    void rewind();
    void forward();
    void updateInfo();
    void updateTime();
    void playPause();

    QList<QAudioOutput*> outputs;
    QString name;
    QString instructions;
    bool stopflag;
    QtSoundException ownExcept;
    QAction* abortAction;
//    QQmlEngine* engine;

private slots:
    void terminated();

signals:
    void doneSignal(QtSoundException);
};

#endif // SOUNDGENERATOR
