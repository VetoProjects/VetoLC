#ifndef PYLIVEINTERPRETER
#define PYLIVEINTERPRETER

#include <Python.h>
#include <QThread>
#include <QAction>
#include <QCoreApplication>
#include "PythonException.hpp"

/**
 * @brief The PyLiveInterpreter class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of QThread that implements an environment for python
 * coding while keeping the main codeeditor thread clean by
 * dispatching.
 */
class PyLiveInterpreter : public QObject{
    Q_OBJECT
public:
    PyLiveInterpreter(char*, char*);
    void run();
    bool updateCode(QString, QString);
    ~PyLiveInterpreter();

private:
    PyObject* execute();
    void exceptionOccurred();
    QString instructions;
    PythonException ownExcept;
    QAction* abortAction;
    PyObject* dict;

private slots:
    void terminated();

signals:
    void doneSignal(PythonException);
};

#endif // PyLiveInterpreter
