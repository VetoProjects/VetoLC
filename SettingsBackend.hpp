#ifndef SETTINGSBACKEND_HPP
#define SETTINGSBACKEND_HPP

#include <QHash>
#include <QVariant>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QWidget>

/**
 * @brief The SettingsBackend class
 *
 * A settings backend. Reads from and writes to the
 * platform independent persistent settings.
 */
class SettingsBackend
{
public:
    SettingsBackend();
    QVariant getSettingsFor(QString, const QVariant &);
    QVariant getSettingsFor(QString, QVariant, int);
	QHash<QString, QVariant> getSettings(int index);
    void addSettings(QString key, QVariant value);
    void saveSettingsFor(int id, const QString &key, const QVariant &value);
    void saveSettingsFor(int id, const QHash<QString, QVariant> &);
    void removeSettings(int);
private:
	QString globalName, globalDir;
};

#endif // SETTINGSBACKEND_HPP
