#ifndef PYSOUNDGENERATOR
#define PYSOUNDGENERATOR

#include <Python.h>
#include <QAction>
#include <QCoreApplication>

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
    void setupPython(QString, QString);
    PyObject* execute_return(QString, QString, QString);
    PyObject* execute(QString);
    void exceptionOccurred();
    void write();
    void stream(PyObject *process);
    int exceptNum;
    QString ownExcept;
    bool ready;
    QAction* abortAction;
    PyObject* main;
    PyObject* dict;
    AudioOutputProcessor* device;

private Q_SLOTS:
    void terminated();
    void setReady();

Q_SIGNALS:
    void doneSignal(QString, int);
};

#endif // PYSOUNDGENERATOR
