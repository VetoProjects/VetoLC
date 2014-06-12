#ifndef PYSOUNDGENERATOR
#define PYSOUNDGENERATOR

#include <Python.h>
#include <QThread>
#include <QAction>
#include <QCoreApplication>
#include "PythonException.hpp"

/**
 * @brief The PySoundGenerator class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
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
    PythonException ownExcept;
    bool triggered;
    QAction* abortAction;
    PyObject* dict;

private slots:
    void terminated();

signals:
    void doneSignal(PythonException);
};

#endif // PYSOUNDGENERATOR
