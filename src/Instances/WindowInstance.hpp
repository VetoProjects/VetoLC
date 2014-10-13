#ifndef EDITORINSTANCE_HPP
#define EDITORINSTANCE_HPP

#include "IInstance.hpp"
#include "../EditorWindow.hpp"

namespace Instances {

class WindowInstance : public IInstance
{
    Q_OBJECT
public:
    WindowInstance(int id, const QHash<QString,QVariant> &settings, QObject *parent = 0);
    ~WindowInstance();
    virtual QString sourceCode() const;
    virtual QString title() const;
    virtual bool close();
    virtual void reportError(const QString &message);
    virtual void reportWarning(const QString &);
    virtual void highlightErroredLine(int);
    virtual void codeStopped();

private:
    WindowInstance& operator=(const WindowInstance& rhs);
    WindowInstance& operator=(WindowInstance&& rhs);

    void createWindow();
    void createWindow(const QHash<QString, QVariant> &settings);
    EditorWindow *_window;

signals:

public slots:
    void gotDestroying(QObject*);
    void gotClosing(EditorWindow *);
    void gotCloseAll(EditorWindow *);
    void gotRunCode(EditorWindow *);
    void gotStopCode(EditorWindow *);
    void gotOpenHelp(EditorWindow *);
    void gotOpenSettings(EditorWindow *);
    void gotChangedSetting(EditorWindow*, const QString &, const QVariant &);
    void gotChangedSettings(EditorWindow*, const QHash<QString,QVariant> &);
    void settingsChanged(const QHash<QString, QVariant> &);
};

}

#endif // EDITORINSTANCE_HPP
