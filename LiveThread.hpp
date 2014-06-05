#ifndef LIVETHREAD
#define LIVETHREAD

#include <QThread>
#include "QtSoundException.hpp"
#include "PythonException.hpp"
#include "QtGlException.hpp"
#include "SoundGenerator.hpp"
#include "Renderer.hpp"
#ifdef WITH_PYTHON
#include "PyLiveInterpreter.hpp"
#include "PySoundGenerator.hpp"
#endif

/**
 * @brief The LiveThread class
 *
 * A subclass of QThread that is optimized for
 * running the interpreters we need.
 */
class LiveThread : public QThread{
    Q_OBJECT
public:
    LiveThread(const long identity, QObject* parent = 0): QThread(parent), ID(identity){}
    virtual void run(){}
    virtual void initialize(const QString &title, const QString &instructions){
        Q_UNUSED(title);
        Q_UNUSED(instructions);
    }
    virtual bool updateCode(const QString &title, const QString &instructions){
        Q_UNUSED(title);
        Q_UNUSED(instructions);
        return false;
    }
    const long ID;
};

class PySoundThread : public LiveThread{
    Q_OBJECT
public:
    PySoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    void run(){
#ifdef WITH_PYTHON
        if(runObj)
            runObj->run();
#endif
    }
    void initialize(const QString &title, const QString &instructions){
#ifdef WITH_PYTHON
        runObj = new PySoundGenerator(title.toLocal8Bit().data(), instructions.toLocal8Bit().data());
        connect(runObj, SIGNAL(doneSignal(PythonException)), this, SLOT(doneSignalReceived(PythonException)));
#else
        Q_UNUSED(title);
        Q_UNUSED(instructions);
        emit doneSignal(this, PythonException("Python is not supported in this version"));
#endif
    }
    bool updateCode(const QString &filename, const QString &code){
#ifdef WITH_PYTHON
       if(runObj)
           return runObj->updateCode(filename, code);
#endif
        return false;
    }
public slots:
    void doneSignalReceived(PythonException exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(PySoundThread*, PythonException);
private:
#ifdef WITH_PYTHON
    PySoundGenerator* runObj;
#endif
};

class PyLiveThread : public LiveThread{
    Q_OBJECT
public:
    PyLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    void run(){
#ifdef WITH_PYTHON
        if(runObj)
            runObj->run();
#endif
    }
    void initialize(const QString &title, const QString &instructions){
#ifdef WITH_PYTHON
        runObj = new PyLiveInterpreter(title.toLocal8Bit().data(), instructions.toLocal8Bit().data());
        connect(runObj, SIGNAL(doneSignal(PythonException)), this, SLOT(doneSignalReceived(PythonException)));
#else
        Q_UNUSED(title);
        Q_UNUSED(instructions);
        emit doneSignal(this, PythonException("Python is not supported in this version"));
#endif
    }
    bool updateCode(const QString &filename, const QString &code){
#ifdef WITH_PYTHON
       if(runObj)
           return runObj->updateCode(filename, code);
#endif
        return false;
    }
public slots:
    void doneSignalReceived(PythonException exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(PyLiveThread*, PythonException);
private:
#ifdef WITH_PYTHON
    PyLiveInterpreter* runObj;
#endif
};

class GlLiveThread: public LiveThread{
    Q_OBJECT
public:
    GlLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    void run(){
        if(runObj)
            runObj->show();
    }
    // No parent object =(
    void initialize(const QString &title, const QString &instructions){
        runObj = new Renderer(title, instructions);
        connect(runObj, SIGNAL(doneSignal(QtGlException)), this, SLOT(doneSignalReceived(QtGlException)));
    }
    bool updateCode(const QString &filename, const QString &code){
        if(runObj)
            return runObj->updateCode(filename, code);
        return false;
    }
public slots:
    void doneSignalReceived(QtGlException exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(GlLiveThread*, QtGlException);
private:
    Renderer* runObj;
};

class QtSoundThread : public LiveThread{
    Q_OBJECT
public:
    QtSoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    void run(){
        if(runObj)
            runObj->run();
    }
    void initialize(const QString &filename, const QString &instructions){
        runObj = new SoundGenerator(filename, instructions);
        connect(runObj, SIGNAL(doneSignal(QtSoundException)), this, SLOT(doneSignalReceived(QtSoundException)));
    }
    bool updateCode(const QString &filename, const QString &code){
        if(runObj)
            return runObj->updateCode(filename, code);
        return false;
    }
public slots:
    void doneSignalReceived(QtSoundException exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(QtSoundThread*, QtSoundException);
private:
    SoundGenerator* runObj;
};

#endif // LIVETHREAD
