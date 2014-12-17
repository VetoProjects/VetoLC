#ifndef LIVETHREAD
#define LIVETHREAD

#ifdef WITH_PYTHON
#include "PyLiveInterpreter.hpp"
#include "PySoundGenerator.hpp"
#endif

#include <QThread>

#include "SoundGenerator.hpp"
#include "Renderer.hpp"

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
    virtual void run() = 0;
    virtual void initialize(const QString &title, const QString &instructions) = 0;
    virtual bool updateCode(const QString &title, const QString &instructions) = 0;
    const long ID;
private:
    LiveThread& operator=(const LiveThread& rhs);
    LiveThread& operator=(LiveThread&& rhs);
};

#ifdef WITH_PYTHON
class PySoundThread : public LiveThread{
    Q_OBJECT
public:
    PySoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    ~PySoundThread(){
        if(runObj)
            delete runObj;
    }
    void run() Q_DECL_OVERRIDE{
        if(runObj)
            runObj->run();
    }
    void initialize(const QString &title, const QString &instructions){
        runObj = new PySoundGenerator(title.toLocal8Bit().data(), instructions.toLocal8Bit().data());
        connect(runObj, SIGNAL(doneSignal(QString, int)), this, SLOT(doneSignalReceived(QString, int)));
    }
    bool updateCode(const QString &filename, const QString &code){
       if(runObj)
           return runObj->updateCode(filename, code);
       return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString exception, int lineno){
        Q_EMIT doneSignal(this, exception, lineno);
    }
Q_SIGNALS:
    void doneSignal(PySoundThread*, QString, int);
private:
    PySoundGenerator* runObj;
};

#else

class PySoundThread : public LiveThread{
    Q_OBJECT
public:
    PySoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){ }
    void run()Q_DECL_OVERRIDE{ }
    void initialize(const QString &, const QString &){
        Q_EMIT doneSignal(this, tr("Python is not supported in this version"), -1);
    }
    bool updateCode(const QString &, const QString &){
        return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString exception, int lineno){
        Q_EMIT doneSignal(this, exception, lineno);
    }
Q_SIGNALS:
    void doneSignal(PySoundThread*, QString, int);
private:
};
#endif

#ifdef WITH_PYTHON
class PyLiveThread : public LiveThread{
    Q_OBJECT
public:
    PyLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    void run() Q_DECL_OVERRIDE{
        if(runObj)
            runObj->run();
    }
    ~PyLiveThread(){
        if(runObj)
            delete runObj;
    }
    void initialize(const QString &title, const QString &instructions){
        runObj = new PyLiveInterpreter(title.toLocal8Bit().data(), instructions.toLocal8Bit().data());
        connect(runObj, SIGNAL(doneSignal(QString, int)), this, SLOT(doneSignalReceived(QString, int)));
    }
    bool updateCode(const QString &filename, const QString &code){
       if(runObj)
           return runObj->updateCode(filename, code);
        return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString exception, int lineno){
        Q_EMIT doneSignal(this, exception, lineno);
    }
Q_SIGNALS:
    void doneSignal(PyLiveThread*, QString, int);
private:
    PyLiveInterpreter* runObj;
};
#else
class PyLiveThread : public LiveThread{
    Q_OBJECT
public:
    PyLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
    }
    ~PyLiveThread(){
    }
    void run() Q_DECL_OVERRIDE{
    }
    void initialize(const QString &title, const QString &instructions){
        Q_UNUSED(title);
        Q_UNUSED(instructions);
        Q_EMIT doneSignal(this, tr("Python is not supported in this version"), -1);
    }
    bool updateCode(const QString &filename, const QString &code){
        Q_UNUSED(filename)
        Q_UNUSED(code)
        return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString exception, int lineno){
        Q_EMIT doneSignal(this, exception, lineno);
    }
Q_SIGNALS:
    void doneSignal(PyLiveThread*, QString, int);
};
#endif

class GlLiveThread: public LiveThread{
    Q_OBJECT
public:
    GlLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    ~GlLiveThread(){
        if(runObj)
            delete runObj;
    }
    void run() Q_DECL_OVERRIDE{
        if(runObj)
            runObj->show();
    }
    // No parent object =(
    void initialize(const QString &title, const QString &instructions){
        runObj = new Renderer(title, instructions);
        connect(runObj, SIGNAL(doneSignal(QString, int)), this, SLOT(doneSignalReceived(QString, int)));

        runObj->resize(800, 600);
        runObj->show();

    }
    bool updateCode(const QString &filename, const QString &code){
        if(runObj)
            return runObj->updateCode(filename, code);
        return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString message, int lineno){
        Q_EMIT errorSignal(this, message, lineno);
    }
Q_SIGNALS:
    void errorSignal(GlLiveThread*, QString, int);
private:
    Renderer* runObj;
};

class QtSoundThread : public LiveThread{
    Q_OBJECT
public:
    QtSoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
        runObj = 0;
    }
    ~QtSoundThread(){
        if(runObj)
            delete runObj;
    }
    void run() Q_DECL_OVERRIDE{
        if(runObj)
            runObj->run();
    }
    void initialize(const QString &filename, const QString &instructions){
        runObj = new SoundGenerator(filename, instructions);
        connect(runObj, SIGNAL(doneSignal(QString)), this, SLOT(doneSignalReceived(QString)));
    }
    bool updateCode(const QString &filename, const QString &code){
        if(runObj)
            return runObj->updateCode(filename, code);
        return false;
    }
public Q_SLOTS:
    void doneSignalReceived(QString exception){
        Q_EMIT doneSignal(this, exception);
    }
Q_SIGNALS:
    void doneSignal(QtSoundThread*, QString);
private:
    SoundGenerator* runObj;
};

#endif // LIVETHREAD
