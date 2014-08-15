#include "WindowInstance.hpp"

namespace Instances {

WindowInstance::WindowInstance(int id, const QHash<QString, QVariant> &settings, QObject *parent) : IInstance(id, parent)
{
    _window = 0;
    createWindow(settings);
}

WindowInstance::~WindowInstance()
{
    delete _window;
}

QString WindowInstance::sourceCode() const
{
    return _window->getSourceCode();
}

QString WindowInstance::title() const
{
    return _window->getTitle();
}

void WindowInstance::reportError(const QString &message)
{
    _window->warningDisplay(message);
}

void WindowInstance::WindowInstance::reportWarning(const QString &text)
{
    _window->showResults(text);
}

void WindowInstance::codeStoped()
{
    _window->codeStopped();
}

bool WindowInstance::close()
{
    disconnect(_window, SIGNAL(closing(EditorWindow*)), this, SLOT(gotClosing(EditorWindow*)));
    if(_window->close()){
        disconnect(_window, SIGNAL(destroyed(QObject*)), this, SLOT(gotDestroying(QObject*))); // Prevent of call gotDestroying
        _window->deleteLater();
        _window = 0;
        return true;
    }
    connect(_window, SIGNAL(closing(EditorWindow*)), this, SLOT(gotClosing(EditorWindow*)));
    return false;
}

void WindowInstance::gotClosing(EditorWindow *)
{
    emit closing(this);
}

void WindowInstance::gotCloseAll(EditorWindow *)
{
    emit closeAll();
}

void WindowInstance::gotOpenHelp(EditorWindow *)
{
    emit openHelp(this);
}

void WindowInstance::gotOpenSettings(EditorWindow *)
{
    emit openSettings(this);
}

void WindowInstance::gotChangedSettings(EditorWindow *, const QHash<QString, QVariant> &settings)
{
    emit changeSettings(this, settings);
}

void WindowInstance::gotChangedSetting(EditorWindow *, const QString &key, const QVariant &value)
{
    emit changeSetting(this, key, value);
}

void WindowInstance::gotDestroying(QObject*)
{
    _window = 0;
    createWindow();
}

void WindowInstance::gotRunCode(EditorWindow *)
{
    emit runCode(this);
}

void WindowInstance::gotStopCode(EditorWindow *)
{
    emit stopCode(this);
}

void WindowInstance::createWindow(const QHash<QString,QVariant> &settings)
{
    if(!_window){
        _window = new EditorWindow(settings);
        connect(_window, SIGNAL(destroyed(QObject*))        , this, SLOT(gotDestroying(QObject*)));
        connect(_window, SIGNAL(closing(EditorWindow*))     , this, SLOT(gotClosing(EditorWindow*)));
        connect(_window, SIGNAL(closeAll(EditorWindow*))    , this, SLOT(gotCloseAll(EditorWindow*)));
        connect(_window, SIGNAL(runCode(EditorWindow*))     , this, SLOT(gotRunCode(EditorWindow*)));
        connect(_window, SIGNAL(stopCode(EditorWindow*))    , this, SLOT(gotStopCode(EditorWindow*)));
        connect(_window, SIGNAL(openHelp(EditorWindow*))    , this, SLOT(gotOpenHelp(EditorWindow*)));
        connect(_window, SIGNAL(openSettings(EditorWindow*)), this, SLOT(gotOpenSettings(EditorWindow*)));
        connect(_window, SIGNAL(changedSetting(EditorWindow*,QString,QVariant)),         this, SLOT(gotChangedSetting(EditorWindow*,QString,QVariant)));
        connect(_window, SIGNAL(changedSettings(EditorWindow*,QHash<QString,QVariant>)), this, SLOT(gotChangedSettings(EditorWindow*,QHash<QString,QVariant>)));
        _window->show();
    }
}

void WindowInstance::createWindow()
{
    if(!_window){
        QHash<QString, QVariant> settings;
        emit getSettings(this, settings);
        createWindow(settings);
    }
}

}
