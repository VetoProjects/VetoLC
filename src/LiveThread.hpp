#ifndef LIVETHREAD
#define LIVETHREAD

#include <QThread>
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
    virtual void run() = 0;
    virtual void initialize(const QString &title, const QString &instructions) = 0;
    virtual bool updateCode(const QString &title, const QString &instructions) = 0;
    const long ID;
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
    void run(){
        if(runObj)
            runObj->run();
    }
    void initialize(const QString &title, const QString &instructions){
        runObj = new PySoundGenerator(title.toLocal8Bit().data(), instructions.toLocal8Bit().data());
        connect(runObj, SIGNAL(doneSignal(QString)), this, SLOT(doneSignalReceived(QString)));
    }
    bool updateCode(const QString &filename, const QString &code){
       if(runObj)
           return runObj->updateCode(filename, code);
       return false;
    }
public slots:
    void doneSignalReceived(QString exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(PySoundThread*, QString);
private:
    PySoundGenerator* runObj;
};

#else

class PySoundThread : public LiveThread{
    Q_OBJECT
public:
    PySoundThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){ }
    void run(){ }
    void initialize(const QString &, const QString &){
        emit doneSignal(this, tr("Python is not supported in this version"));
    }
    bool updateCode(const QString &, const QString &){
        return false;
    }
public slots:
    void doneSignalReceived(QString exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(PySoundThread*, QString);
private:
};
#endif

class PyLiveThread : public LiveThread{
    Q_OBJECT
public:
    PyLiveThread(const long identity, QObject* parent = 0) : LiveThread(identity, parent){
#ifdef WITH_PYTHON
        runObj = 0;
#endif
    }
    ~PyLiveThread(){
#ifdef WITH_PYTHON
        if(runObj)
            delete runObj;
#endif
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
        connect(runObj, SIGNAL(doneSignal(QString)), this, SLOT(doneSignalReceived(QString)));
#else
        Q_UNUSED(title);
        Q_UNUSED(instructions);
        emit doneSignal(this, tr("Python is not supported in this version");
#endif
    }
    bool updateCode(const QString &filename, const QString &code){
#ifdef WITH_PYTHON
       if(runObj)
           return runObj->updateCode(filename, code);
#else
        Q_UNUSED(filename)
        Q_UNUSED(code)
#endif
        return false;
    }
public slots:
    void doneSignalReceived(QString exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(PyLiveThread*, QString);
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
    ~GlLiveThread(){
        if(runObj)
            delete runObj;
    }
    void run(){
        if(runObj)
            runObj->show();
    }
    // No parent object =(
    void initialize(const QString &title, const QString &instructions){
        runObj = new Renderer(title, instructions);
        connect(runObj, SIGNAL(doneSignal(QString)), this, SLOT(doneSignalReceived(QString)));
    }
    bool updateCode(const QString &filename, const QString &code){
        if(runObj)
            return runObj->updateCode(filename, code);
        return false;
    }
public slots:
    void doneSignalReceived(QString exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(GlLiveThread*, QString);
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
    void run(){
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
public slots:
    void doneSignalReceived(QString exception){
        emit doneSignal(this, exception);
    }
signals:
    void doneSignal(QtSoundThread*, QString);
private:
    SoundGenerator* runObj;
};

#endif // LIVETHREAD
