#include "SettingsBackend.hpp"
#include <QDebug>

/**
 * @brief SettingsBackend::SettingsBackend
 *
 * The constructor of the SettingsBackend class.
 * Sets name and directory of the settings.
 */
SettingsBackend::SettingsBackend(){
    globalName = "VeTo";
	globalDir = "Live Code Editor";
}

/**
 * @brief SettingsBackend::getSettingsFor
 * @param key
 * @param defaultOption
 * @param id
 * @return the settings requested
 *
 * Takes a key, a default option and the index of
 * the requesting child window. Gets the settings
 * for the key or returns defaultOption.
 */
QVariant SettingsBackend::getSettingsFor(QString key,
                                         QVariant defaultOption,
                                         int id){
    QString relevantSubdir(globalDir + "/" + QString::number(id));
    QSettings set(globalName, relevantSubdir);
    return set.value(key, defaultOption);

}

/**
 * @brief SettingsBackend::getSettingsFor
 * @param key
 * @param defaultOption
 * @return the settings requested
 *
 * Takes a key and a default option. Gets the
 * settings for the key or returns defaultOption.
 */
QVariant SettingsBackend::getSettingsFor(QString key, const QVariant &defaultOption){
    QSettings settings(globalName, globalDir);
    return settings.value(key, defaultOption);
}

/**
 * @brief SettingsBackend::getSettings
 * @param id
 * @return the settings as a QHash
 *
 * gets the settings for a child window and
 * translates it to a QHash that is returned.
 */
QHash<QString, QVariant> SettingsBackend::getSettings(int id){
    QHash<QString, QVariant> settings;
    QString relevantSubdir(globalDir + "/" + QString::number(id));
    QSettings set(globalName, relevantSubdir);
    foreach(const QString &key, set.childKeys())
        settings.insert(key, set.value(key));
    return settings;
}

/**
 * @brief SettingsBackend::addSettings
 * @param key
 * @param value
 *
 * Adds an entry to the global settings.
 */
void SettingsBackend::addSettings(QString key, QVariant value){
    QSettings settings(globalName, globalDir);
    settings.setValue(key, value);
}



/**
 * @brief SettingsBackend::saveSettings
 * @param id
 * @param key
 * @param value
 *
 * Saves the settings of a instance.
 */
void SettingsBackend::saveSettingsFor(int id, const QString &key, const QVariant &value)
{
    QString relevantSubdir(globalDir + "/" + QString::number(id));
    QSettings set(globalName, relevantSubdir);
    set.setValue(key, value);
}

void SettingsBackend::saveSettingsFor(int id, const QHash<QString, QVariant> &settings){
//    qDebug() << "save settings for" << id << ":" << settings;
    QString relevantSubdir(globalDir + "/" + QString::number(id));
    QSettings set(globalName, relevantSubdir);
    for(QString &key : settings.keys())
        set.setValue(key, settings[key]);
}

/**
 * @brief SettingsBackend::removeSettings
 * @param id
 *
 * Removes the settings of a child.
 */
void SettingsBackend::removeSettings(int id){
    QString relevantSubdir(globalDir + "/" + QString::number(id));
    QSettings set(globalName, relevantSubdir);
    set.clear();
}
