#include "SettingsWindow.hpp"

/**
 * @brief SettingsWindow::SettingsWindow
 *
 * The constructor of the settings window.
 * Sets up the SIGNALS, UI and the tabs.
 */
SettingsWindow::SettingsWindow(int subDirNum){
    subDir = subDirNum;
    settingsDict = SettingsBackend::getSettings(subDirNum);

    tabs = new QTabWidget;
    layout = new LayoutTab(&settingsDict, this);
    behaviour = new BehaviourTab(&settingsDict, this);
    changed = false;
    tabs->addTab(layout, "Layout");
    connect(layout, SIGNAL(contentChanged()), this, SLOT(changedTrue()));
    tabs->addTab(behaviour, "Behaviour");
    connect(behaviour, SIGNAL(contentChanged()), this, SLOT(changedTrue()));

    auto* horizontal = new QHBoxLayout();
    horizontal->addWidget(tabs, 1);

    auto* applyBut = new QPushButton(tr("Apply Changes"));
    auto* applycloseBut = new QPushButton(tr("Apply and Exit"));
    auto* closeBut = new QPushButton(tr("Close"));
    connect(applyBut, SIGNAL(clicked()), this, SLOT(apply()));
    connect(applycloseBut, SIGNAL(clicked()), this, SLOT(applyClose()));
    connect(closeBut, SIGNAL(clicked()), this, SLOT(tryClose()));

    auto* buttons = new QHBoxLayout();
    buttons->addStretch(1);
    buttons->addWidget(applyBut);
    buttons->addWidget(applycloseBut);
    buttons->addWidget(closeBut);

    auto* main = new QVBoxLayout();
    main->addLayout(horizontal);
    main->addStretch(1);
    main->addSpacing(12);
    main->addLayout(buttons);

    setLayout(main);
    setWindowTitle(tr("VeToLC | Editor Settings"));
}

/**
 * @brief SettingsWindow::~SettingsWindow
 *
 * Destructor of the SettingsWindow class.
 * Cleans up after the window was close.
 */
SettingsWindow::~SettingsWindow(){
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
        QApplication::setStyle(settingsDict["Design"].toString());
        repaint();
        update();

        SettingsBackend::addSettings("Design", settingsDict["Design"]);
        SettingsBackend::addSettings("OpenFiles", settingsDict["OpenFiles"]);

        settingsDict.remove("Design");
        settingsDict.remove("OpenFiles");

        SettingsBackend::saveSettingsFor(subDir, settingsDict);

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
        auto question = QMessageBox::warning(this, tr("VeToLC"),
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
 *
 * setter for the changed flag. Sets the flag to true.
 * This will result in the apply function having effect.
 */
void SettingsWindow::changedTrue(){
    changed = true;
}
