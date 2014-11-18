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
    static QVariant getSettingsFor(QString, const QVariant &);
    static QVariant getSettingsFor(const QString, const QVariant, const int);
    static QHash<QString, QVariant> getSettings(const int index);
    static void addSettings(const QString key, const QVariant value);
    static void saveSettingsFor(const int id, const QString &key, const QVariant &value);
    static void saveSettingsFor(const int id, const QHash<QString, QVariant> &);
    static void removeSettings(const int);
private:
    static const QString globalName,
                         globalDir;
};

#endif // SETTINGSBACKEND_HPP
