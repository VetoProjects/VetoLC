#ifndef SETTINGSWINDOW
#define SETTINGSWINDOW

#include <QDialog>
#include <QStackedWidget>
#include <QTabWidget>
#include "SettingsTab.hpp"

/**
 * @brief The SettingsWindow class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of QDialog that implements a Settings Window
 * in which various configurations regarding behaviour and
 * layout of the editor can be made.
 */
class SettingsWindow : public QDialog{
Q_OBJECT

public:
    SettingsWindow(int);
    ~SettingsWindow();

private slots:
   void apply();
   void applyClose();
   void tryClose();
   void setChanged();

private:
    void addButtons();

    bool changed;
    QListWidget* content;
    QTabWidget* tabs;
    LayoutTab* layout;
    BehaviourTab* behaviour;
    QSettings* settings;
};

#endif // SETTINGSWINDOW
