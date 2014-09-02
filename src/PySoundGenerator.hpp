#ifndef PYSOUNDGENERATOR
#define PYSOUNDGENERATOR

#include <Python.h>
#include <QThread>
#include <QAction>
#include <QCoreApplication>
#include <QIODevice>
#include "AudioOutputProcessor.hpp"

/**
 * @brief The PySoundGenerator class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QThread that implements an environment for sound
 * live coding while keeping the main codeeditor thread clean by
 * dispatching. Python-based.
 */
class PySoundGenerator : public QObject{
Q_OBJECT
public:
    PySoundGenerator(char*, char*);
    void run();
    bool updateCode(QString, QString);
    ~PySoundGenerator();

private:
    PyObject* execute(QString instruct);
    void exceptionOccurred();
    void write();
    void stream(PyObject *process);
    int exceptNum;
    QString ownExcept;
    bool ready;
    QAction* abortAction;
    PyObject* sys;
    PyObject* audio;
    PyObject* dict;
    AudioOutputProcessor* device;

private slots:
    void terminated();
    void setReady();

signals:
    void doneSignal(QString, int);
};

#endif // PYSOUNDGENERATOR
