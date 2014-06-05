#ifndef SETTINGTABS
#define SETTINGTABS

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QButtonGroup>
#include <QMessageBox>
#include <QSettings>

/**
 * @brief The SettingsTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of QWidget that implements a base class(not abstract)
 *
 */
class SettingsTab : public QWidget{
Q_OBJECT
public:
    SettingsTab(QWidget* parent = 0, int = 0);
    ~SettingsTab();
    QHash<QString, int> getSettings();
    static QHash<QString, int>* settings;
private:
    static int count;
signals:
    void contentChanged();
};

/**
 * @brief The LayoutTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of SettingsTab that implements one of the tabs
 * of the SettingsWindow in which all configurations regarding
 * layout can be found.
 */
class LayoutTab : public SettingsTab{
Q_OBJECT
public:
    LayoutTab(QWidget* parent = 0, int = 0);
private slots:
    void addLayout();
    void designSettings(int);
    void hlSettings(int);
    void languageSettings(int);
};

/**
 * @brief The LayoutTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of SettingsTab that implements one of the tabs
 * of the SettingsWindow in which all configurations regarding
 * behaviour can be found.
 */
class BehaviourTab : public SettingsTab{
Q_OBJECT
public:
    BehaviourTab(QWidget* parent = 0, int = 0);
private:
    void addLayout();
private slots:
    void openSlot(bool);
    void sizeSlot(bool);
    void rememberCompilerSlot(bool);
    void useCompilerSlot(int);
};

#endif // SETTINGTABS
