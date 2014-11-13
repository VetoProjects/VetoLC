#ifndef PYLIVEINTERPRETER
#define PYLIVEINTERPRETER

#include <Python.h>
#include <frameobject.h>
#include <QThread>
#include <QAction>
#include <QCoreApplication>
#include "PythonException.hpp"

/**
 * @brief The PyLiveInterpreter class
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
    QString ownExcept;
    int exceptNum;
    QAction* abortAction;
    PyObject* dict;

private Q_SLOTS:
    void terminated();

signals:
    void doneSignal(QString, int);
};

#endif // PyLiveInterpreter
