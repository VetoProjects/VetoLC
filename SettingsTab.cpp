#include "SettingsTab.hpp"

/**
 * @brief SettingsTab::settings
 *
 * needed because the variable is static.
 */
QHash<QString, int>* SettingsTab::settings;

/**
 * @brief SettingsTab::count
 *
 * needed because the variable is static.
 */
int SettingsTab::count = 0;

/**
 * @brief SettingsTab::SettingsTab
 *
 * The constructor of the SettingsTab class.
 * Translates the QSettings into a list of strings and integers
 * we can make use of.
 */
SettingsTab::SettingsTab(QWidget* parent, int subDirNum) : QWidget(parent){
    if(count == 0){
    settings = new QHash<QString, int>();
    QString subdirectory("Live Code Editor/");
    subdirectory.append(QString::number(subDirNum));
    QSettings set("VeTo", subdirectory);
    foreach(const QString &key, set.childKeys())
        settings->insert(key, set.value(key).toInt());
    }
    count++;
}

SettingsTab::~SettingsTab(){
    if(--count == 0)
        delete settings;
}

/**
 * @brief SettingsTab::getSettings
 * @return a copy of the internal settings hashlist
 *
 * a getter for the settings.
 */
QHash<QString, int> SettingsTab::getSettings(){
    return *settings;
}

/**
 * @brief LayoutTab::LayoutTab
 *
 * Constructor of the LayoutTab class.
 * Calls the addLayout member function.
 */
LayoutTab::LayoutTab(QWidget* parent, int subDirNum) : SettingsTab(parent, subDirNum){
    addLayout();
}

/**
 * @brief LayoutTab::addLayout
 *
 * Creates the layout tab UI and makes it interactive.
 */
void LayoutTab::addLayout(){
    QGroupBox* design = new QGroupBox(tr("Design"));

    QLabel* designBoxLabel = new QLabel(tr("Design:"));
    QComboBox* designBox = new QComboBox;
    designBox->addItem(tr("Standard"));
    designBox->addItem(tr("Red"));
    designBox->addItem(tr("Black"));
    designBox->addItem(tr("Brown"));
    designBox->addItem(tr("White"));

    int designConfig = settings->find("Design").value();
    if(designConfig >= 0 || designConfig <= 4)
        designBox->setCurrentIndex(designConfig);

    connect(designBox, SIGNAL(currentIndexChanged(int)), this, SLOT(designSettings(int)));

    QHBoxLayout* verticalDesign = new QHBoxLayout;
    verticalDesign->addWidget(designBoxLabel);
    verticalDesign->addWidget(designBox);

    QVBoxLayout* horizontalDesign = new QVBoxLayout;
    horizontalDesign->addLayout(verticalDesign);
    design->setLayout(horizontalDesign);

    QGroupBox* highlighting = new QGroupBox(tr("Highlighting"));

    QLabel* hlBoxLabel = new QLabel(tr("Highlighting:"));
    QComboBox* hlBox = new QComboBox;
    hlBox->addItem(tr("GLSL"));
    hlBox->addItem(tr("Python"));
    hlBox->addItem(tr("QT"));
    hlBox->addItem(tr("Variable Names"));    
    hlBox->addItem(tr("None"));

    int hlConfig = settings->find("Highlighting").value();
    if(hlConfig >= 0 || hlConfig <= 4)
        hlBox->setCurrentIndex(hlConfig);

    connect(hlBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(hlSettings(int)));

    QHBoxLayout*  verticalHl = new QHBoxLayout;
    verticalHl->addWidget(hlBoxLabel);
    verticalHl->addWidget(hlBox);

    QVBoxLayout* horizontalHl = new QVBoxLayout;
    horizontalHl->addLayout(verticalHl);
    highlighting->setLayout(horizontalHl);

    QGroupBox* language = new QGroupBox(tr("Language"));

    QLabel* languageBoxLabel = new QLabel(tr("Language:"));
    QComboBox* languageBox = new QComboBox;
    languageBox->addItem(tr("English"));
    languageBox->addItem(tr("German"));

    int languageConfig = settings->find("Language").value();
    if(languageConfig >= 0 || languageConfig <= 1)
        languageBox->setCurrentIndex(languageConfig);

    connect(languageBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(languageSettings(int)));

    QHBoxLayout*  verticalLanguage = new QHBoxLayout;
    verticalLanguage->addWidget(languageBoxLabel);
    verticalLanguage->addWidget(languageBox);

    QVBoxLayout* horizontalLanguage = new QVBoxLayout;
    horizontalLanguage->addLayout(verticalLanguage);
    language->setLayout(horizontalLanguage);

    QVBoxLayout* main = new QVBoxLayout;
    main->addWidget(design);
    main->addWidget(highlighting);
    main->addWidget(language);
    main->addStretch(1);
    setLayout(main);
}

/**
 * @brief LayoutTab::designSettings
 * @param index
 *
 * SLOT that reacts to the currentIndexChanged SIGNAL
 * of the Design drop down list. Changes it according to the users input.
 */
void LayoutTab::designSettings(int index){
    settings->insert("Design", index);
    emit contentChanged();
}


/**
 * @brief LayoutTab::hlSettings
 * @param index
 *
 * SLOT that reacts to the currentIndexChanged SIGNAL
 * of the Highlighting drop down list. Changes it according to the users input.
 */
void LayoutTab::hlSettings(int index){
    settings->insert("Highlighting", index);
    emit contentChanged();
}


/**
 * @brief LayoutTab::languageSettings
 * @param index
 *
 * SLOT that reacts to the currentIndexChanged SIGNAL
 * of the Language drop down list. Changes it according to the users input.
 */
void LayoutTab::languageSettings(int index){
    settings->insert("Language", index);
    emit contentChanged();
}

/**
 * @brief BehaviourTab::BehaviourTab
 *
 * Construcotr of the BehaviourTab class.
 * Calls addLayout().
 */
BehaviourTab::BehaviourTab(QWidget* parent, int subDirNum) : SettingsTab(parent, subDirNum){
    addLayout();
}

/**
 * @brief BehaviourTab::addLayout
 *
 * Creates the layout tab UI and makes it interactive.
 */
void BehaviourTab::addLayout(){
    QGroupBox* startup = new QGroupBox(tr("Startup Behaviour"));
    QCheckBox* openCheck = new QCheckBox(tr("Open Last Files On Startup"));
    QCheckBox* sizeCheck = new QCheckBox(tr("Remember Size Of Application"));

    if(settings->find("OpenFiles").value() == 1)
        openCheck->toggle();
    else
        settings->insert("OpenFiles", 0);

    if(settings->find("RememberSize").value() == 1)
        sizeCheck->toggle();
    else
        settings->insert("RememberSize", 0);

    QButtonGroup* startupCompiler = new QButtonGroup(this);
    QCheckBox* rememberCompiler = new QCheckBox(tr("Remember Compiler that was used last"));
    QCheckBox* askForCompiler = new QCheckBox(tr("Always ask"));
    startupCompiler->addButton(rememberCompiler);
    startupCompiler->addButton(askForCompiler);

    if(settings->find("RememberCompiler").value() == 1)
        rememberCompiler->toggle();
    else{
        settings->insert("RememberCompiler", 0);
        askForCompiler->toggle();
    }

    connect(openCheck, SIGNAL(toggled(bool)), this, SLOT(openSlot(bool)));
    connect(sizeCheck, SIGNAL(toggled(bool)), this, SLOT(sizeSlot(bool)));
    connect(rememberCompiler, SIGNAL(toggled(bool)), this, SLOT(rememberCompilerSlot(bool)));

    QGroupBox* compiler = new QGroupBox(tr("Compiler to be used:"));

    QLabel* compilerChoiceLabel = new QLabel(tr("Compiler:"));
    QComboBox* compilerChoice = new QComboBox;
    compilerChoice->addItem(tr("AudioPython"));
    compilerChoice->addItem(tr("AudioQT"));
    compilerChoice->addItem(tr("GLSL"));
    compilerChoice->addItem(tr("Python (Regular)"));

    int useCompilerConfig = settings->find("UseCompiler").value();
    if(useCompilerConfig >= 0 || useCompilerConfig <= 2)
        compilerChoice->setCurrentIndex(useCompilerConfig);

    connect(compilerChoice, SIGNAL(currentIndexChanged(int)), this, SLOT(useCompilerSlot(int)));

    QVBoxLayout* startupLayout = new QVBoxLayout;
    startupLayout->addWidget(openCheck);
    startupLayout->addWidget(sizeCheck);
    startupLayout->addSpacing(10);
    startupLayout->addWidget(rememberCompiler);
    startupLayout->addWidget(askForCompiler);
    startup->setLayout(startupLayout);

    QVBoxLayout* compilerLayout = new QVBoxLayout;
    compilerLayout->addWidget(compilerChoiceLabel);
    compilerLayout->addWidget(compilerChoice);
    compiler->setLayout(compilerLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(startup);
    mainLayout->addWidget(compiler);
    mainLayout->addSpacing(12);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

/**
 * @brief BehaviourTab::openSlot
 * @param toggled
 *
 * SLOT that reacts to the toggled() SIGNAL of
 * openCheck. Writes change to Hashlist and emits
 * a contentChanged signal.
 */
void BehaviourTab::openSlot(bool toggled){
    settings->insert("OpenFiles", (int)toggled);
    emit contentChanged();
}

/**
 * @brief BehaviourTab::sizeSlot
 * @param toggled
 *
 * SLOT that reacts to the toggled() SIGNAL of
 * sizeCheck. Writes change to Hashlist and emits
 * a contentChanged signal.
 */
void BehaviourTab::sizeSlot(bool toggled){
    settings->insert("RememberSize", (int)toggled);
    emit contentChanged();
}

/**
 * @brief BehaviourTab::rememberCompilerSlot
 * @param toggled
 *
 * SLOT that reacts to the toggled() SIGNAL of
 * rememberCompiler. Writes change to Hashlist and emits
 * a contentChanged signal.
 */
void BehaviourTab::rememberCompilerSlot(bool toggled){
    settings->insert("RememberCompiler", (int)toggled);
    emit contentChanged();
}

/**
 * @brief BehaviourTab::useCompilerSlot
 * @param index
 *
 * SLOT that reacts to the currentIndexChanged SIGNAL of
 * the Compiler drop down list. Writes change to Hashlist
 * and emits a contentChanged signal.
 */
void BehaviourTab::useCompilerSlot(int index){
    settings->insert("UseCompiler", index);
    emit contentChanged();
}
