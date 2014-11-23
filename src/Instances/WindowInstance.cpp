#include "WindowInstance.hpp"

namespace Instances {

/**
 * @brief WindowInstance::WindowInstance
 * @param id
 * @param settings
 * @param parent
 *
 * Constructor of the delegate.
 */
WindowInstance::WindowInstance(int id, const QHash<QString, QVariant> &settings, QObject *parent) : IInstance(id, parent)
{
    _window = 0;
    createWindow(settings);
}

/**
 * @brief WindowInstance::~WindowInstance
 *
 * Destructor of the delegate.
 */
WindowInstance::~WindowInstance()
{
    delete _window;
}


/**
 * @brief WindowInstance::sourceCode
 * @return the editor contents as string
 *
 * Gets the Editor Content as a QString.
 */
QString WindowInstance::sourceCode() const
{
    return _window->getSourceCode();
}

/**
 * @brief WindowInstance::title
 * @return file name as a QString
 *
 * Gets the file name as a QString.
 */
QString WindowInstance::title() const
{
    return _window->getTitle();
}

/**
 * @brief WindowInstance::reportError
 * @param message
 *
 * Displays error message in editor.
 */
void WindowInstance::reportError(const QString &message)
{
    _window->warningDisplay(message);
}

/**
 * @brief WindowInstance::reportWarning
 * @param text
 *
 * Displays warning message in editor.
 */
void WindowInstance::reportWarning(const QString &text)
{
    _window->showResults(text);
}

/**
 * @brief WindowInstance::highlightErroredLine
 * @param lineno
 *
 * Highlight line number lineno as errored.
 */
void WindowInstance::highlightErroredLine(int lineno)
{
    _window->highlightErroredLine(lineno);
}

/**
 * @brief WindowInstance::codeStopped
 *
 * Signals that execution of the editor code has
 * finsihed.
 */
void WindowInstance::codeStopped()
{
    _window->codeStopped();
}

/**
 * @brief WindowInstance::close
 * @return whether closing has succeeded
 *
 * Closes the editor window and return a success flag.
 */
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

/**
 * @brief WindowInstance::gotClosing
 *
 * Signals that the editor requested closing.
 */
void WindowInstance::gotClosing(EditorWindow *)
{
    Q_EMIT closing(this);
}

/**
 * @brief WindowInstance::gotCloseAll
 *
 * Signals that the editor requested closing of all windows.
 */
void WindowInstance::gotCloseAll(EditorWindow *)
{
    Q_EMIT closeAll();
}

/**
 * @brief WindowInstance::gotOpenHelp
 *
 * Signals that the editor requested a help window.
 */
void WindowInstance::gotOpenHelp(EditorWindow *)
{
    Q_EMIT openHelp(this);
}

/**
 * @brief WindowInstance::gotOpenSettings
 *
 * Signals that the editor requested a settings window.
 */
void WindowInstance::gotOpenSettings(EditorWindow *)
{
    Q_EMIT openSettings(this);
}

/**
 * @brief WindowInstance::gotChangedSettings
 * @param settings
 *
 * Signals that the editors' settings have changed(completely).
 */
void WindowInstance::gotChangedSettings(EditorWindow *, const QHash<QString, QVariant> &settings)
{
    Q_EMIT changeSettings(this, settings);
}

/**
 * @brief WindowInstance::gotChangedSetting
 * @param key
 * @param value
 *
 * Signals that the editors' settings have changed(singla value).
 */
void WindowInstance::gotChangedSetting(EditorWindow *, const QString &key, const QVariant &value)
{
    Q_EMIT changeSetting(this, key, value);
}

void WindowInstance::gotDestroying(QObject*)
{
    _window = 0;
    createWindow();
}

/**
 * @brief WindowInstance::gotRunCode
 *
 * Signals that the editor requested a run.
 */
void WindowInstance::gotRunCode(EditorWindow *)
{
    Q_EMIT runCode(this);
}

/**
 * @brief WindowInstance::gotStopCode
 *
 * Signals that the editor requested stop of execution.
 */
void WindowInstance::gotStopCode(EditorWindow *)
{
    Q_EMIT stopCode(this);
}

/**
 * @brief WindowInstance::createWindow
 * @param settings
 *
 * Creates a window with settings.
 */
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

/**
 * @brief WindowInstance::createWindow
 *
 * Creates a window without settings.
 */
void WindowInstance::createWindow()
{
    if(!_window){
        QHash<QString, QVariant> settings;
        Q_EMIT getSettings(this, settings);
        createWindow(settings);
    }
}

}
