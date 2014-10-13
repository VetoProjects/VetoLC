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
#include <QRadioButton>

/**
 * @brief The SettingsTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QWidget that implements a base class(not abstract)
 *
 */
class SettingsTab : public QWidget{
Q_OBJECT
public:
    SettingsTab(QHash<QString,QVariant> *Settings, QWidget* parent = 0);
signals:
    void contentChanged();
protected:
    QHash<QString, QVariant> *settings;
};

/**
 * @brief The LayoutTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of SettingsTab that implements one of the tabs
 * of the SettingsWindow in which all configurations regarding
 * layout can be found.
 */
class LayoutTab : public SettingsTab{
Q_OBJECT
public:
    LayoutTab(QHash<QString, QVariant> *Settings, QWidget* parent = 0);
    ~LayoutTab();
private slots:
    void addLayout();
    void designSettings(QString);
    void hlSettings(int);
    void languageSettings(int);
private:
    QGroupBox* design;
    QLabel* designBoxLabel;
    QComboBox* designBox;
    QHBoxLayout* verticalDesign;
    QVBoxLayout* horizontalDesign;
    QGroupBox* highlighting;
    QLabel* hlBoxLabel;
    QComboBox* hlBox;
    QHBoxLayout* verticalHl;
    QVBoxLayout* horizontalHl;
    QGroupBox* language;
    QComboBox* languageBox;
    QLabel* languageBoxLabel;
    QHBoxLayout* verticalLanguage;
    QVBoxLayout* horizontalLanguage;
    QVBoxLayout* main;
};

/**
 * @brief The LayoutTab class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of SettingsTab that implements one of the tabs
 * of the SettingsWindow in which all configurations regarding
 * behaviour can be found.
 */
class BehaviourTab : public SettingsTab{
Q_OBJECT
public:
    BehaviourTab(QHash<QString, QVariant> *Settings, QWidget* parent = 0);
    ~BehaviourTab();
private:
    void addLayout();
private slots:
    void openSlot(bool);
    void sizeSlot(bool);
    void rememberCompilerSlot(bool);
    void useCompilerSlot(int);
    void replaceSlot(bool);
private:
    QGroupBox* startup;
    QCheckBox* openCheck;
    QCheckBox* sizeCheck;
    QButtonGroup* startupCompiler;
    QRadioButton* rememberCompiler;
    QRadioButton* askForCompiler;
    QGroupBox* editor;
    QCheckBox* replaceTabs;
    QGroupBox* compiler;
    QLabel* compilerChoiceLabel;
    QComboBox* compilerChoice;
    QVBoxLayout* startupLayout;
    QVBoxLayout* compilerLayout;
    QVBoxLayout* editorLayout;
    QVBoxLayout* mainLayout;
};

#endif // SETTINGTABS
