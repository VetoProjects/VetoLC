#include "SettingsWindow.hpp"

/**
 * @brief SettingsWindow::SettingsWindow
 *
 * The constructor of the settings window.
 * Sets up the SIGNALS, UI and the tabs.
 */
SettingsWindow::SettingsWindow(int subDirNum){
    QString subdirectory("Live Code Editor/");
    subdirectory.append(QString::number(subDirNum));
    settings = new QSettings("VeTo", subdirectory);
    tabs = new QTabWidget;
    layout = new LayoutTab(this);
    behaviour = new BehaviourTab(this);
    changed = false;
    tabs->addTab(layout, "Layout");
    connect(layout, SIGNAL(contentChanged()), this, SLOT(setChanged()));
    tabs->addTab(behaviour, "Behaviour");
    connect(behaviour, SIGNAL(contentChanged()), this, SLOT(setChanged()));

    QHBoxLayout* horizontal = new QHBoxLayout;
    horizontal->addWidget(tabs, 1);

    QPushButton* applyBut = new QPushButton(tr("Apply Changes"));
    QPushButton* applycloseBut = new QPushButton(tr("Apply and Exit"));
    QPushButton* closeBut = new QPushButton(tr("Close"));
    connect(applyBut, SIGNAL(clicked()), this, SLOT(apply()));
    connect(applycloseBut, SIGNAL(clicked()), this, SLOT(applyClose()));
    connect(closeBut, SIGNAL(clicked()), this, SLOT(tryClose()));

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addStretch(1);
    buttons->addWidget(applyBut);
    buttons->addWidget(applycloseBut);
    buttons->addWidget(closeBut);

    QVBoxLayout* main = new QVBoxLayout;
    main->addLayout(horizontal);
    main->addStretch(1);
    main->addSpacing(12);
    main->addLayout(buttons);

    setLayout(main);
    setWindowTitle(tr("VeTo Live Coding Editor Settings"));
}

/**
 * @brief SettingsWindow::~SettingsWindow
 *
 * Destructor of the SettingsWindow class.
 * Cleans up after the window was close.
 */
SettingsWindow::~SettingsWindow(){
    delete settings;
    delete layout;
    delete behaviour;
    delete tabs;
}

/**
 * @brief SettingsWindow::apply
 *
 * applies the changes made in the window.
 */
void SettingsWindow::apply(){
    if(changed){
        QHash<QString, int> tabSettings = layout->getSettings();
        tabSettings.unite(behaviour->getSettings());
        QHash<QString, int>::iterator i;
        for(i = tabSettings.begin(); i != tabSettings.end(); i++)
            settings->setValue(i.key(), i.value());
        changed = false;
    }
}

/**
 * @brief SettingsWindow::applyClose
 *
 * calls apply() and close(), thus saving the changes and exiting the window.
 */
void SettingsWindow::applyClose(){
    apply();
    close();
}

/**
 * @brief SettingsWindow::tryClose
 *
 * if there are unsaved changes, this method will create a checkbox
 * asking the user whether he/she wants to quit. Otherwise, it will just exit.
 */
void SettingsWindow::tryClose(){
    if(changed){
        QMessageBox::StandardButton question;
        question = QMessageBox::warning(this, tr("VeTo Live Coding Editor"),
                                tr("The settings have been modified"
                                   " but are unsaved.\n"
                                "Do you want to save your changes?"),
                                QMessageBox::Save | QMessageBox::Discard
                                | QMessageBox::Cancel);
        if(question == QMessageBox::Save)
            apply();
        else if(question == QMessageBox::Cancel)
            return;
    }
    close();
}

/**
 * @brief SettingsWindow::setChanged
 * @param set
 *
 * setter for the changed flag. Sets the flag to the bool provided.
 * This will result in the apply function having effect.
 */
void SettingsWindow::setChanged(){
    changed = true;
}
