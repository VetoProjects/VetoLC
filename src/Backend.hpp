#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QAction>
#include <QTextStream>
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QException>
#include <QObject>
#include <QSettings>
#include <QDir>
#include "SettingsBackend.hpp"
#include "SettingsWindow.hpp"
#include "LiveThread.hpp"
#include "Instances/IInstance.hpp"
using namespace Instances;


/**
 * @brief The Backend class
 *
 * The heart and soul of the eidtors functionality.
 * Is connected to all the other parts of the application
 * (through SIGNALs as well as references) and keeps track
 * of all the windows and code evaluation threads that are created
 * and deleted.
 */
class Backend : public QObject
{
    Q_OBJECT
public:
    static QDir directoryOf(const QString&);
    explicit Backend(QObject *parent = 0);
    ~Backend();
    void addInstance(IInstance *, bool = true);
    void createChild();
    void childExited(IInstance *, QString);
    bool isLast();
    bool removeInstance(IInstance*, bool = true);
    bool removeInstance(int, bool = true);
    void removeSettings(IInstance*);
    void saveAllSettings();
    void saveSettings(IInstance *, QString);
    QHash<QString, QVariant> getSettings(IInstance *);
    QHash<QString, QVariant> getSettings(int id);
    int nextID();
    QList<int> loadIds();

signals:
    void warningSignal(QWidget*, QString);
    void closeAction();
    void saveAction();
    void showResults(const QString &);
    void childDoSaveSettings();

public slots:
    void settingsWindowRequested(IInstance*);
    void openHelp(IInstance *);
    void instanceClosing(IInstance *);
    void instanceDestroyed(QObject*);
    void instanceRunCode(IInstance *);
    void instanceStopCode(IInstance *);
    void instanceChangedSetting(IInstance *, const QString &key, const QVariant &value);
    void instanceRequestSetting(IInstance *, const QString &key, QVariant &value);
    void instanceChangedSettings(IInstance *, const QHash<QString, QVariant> &);
    void instanceRequestSettings(IInstance *, QHash<QString, QVariant> &);
//    void instanceRemoveID(IInstance *instance);
    void childSaidCloseAll();

    void getExecutionResults(QtSoundThread*, QString);
    void getExecutionResults(PySoundThread*, QString, int);
    void getExecutionResults(PyLiveThread*, QString, int);
    void getExecutionResults(GlLiveThread*, QString);

private:
    void runPyFile(IInstance *);
    void runQtSoundFile(IInstance *);
    void runGlFile(IInstance *);
    void runPySoundFile(IInstance *);
    void terminateThread(long id);
    SettingsBackend settings;
    QList<int> ids;
    QHash<long, IInstance*> instances;
    QHash<long, LiveThread*> threads;
    void saveIDs();
};

#endif // BACKEND_HPP
