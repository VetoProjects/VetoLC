#include "SettingsTab.hpp"
#include <QStyleFactory>

/**
 * @brief SettingsTab::SettingsTab
 *
 * The constructor of the SettingsTab class.
 * Translates the QSettings into a list of strings and integers
 * we can make use of.
 */
SettingsTab::SettingsTab(QHash<QString, QVariant> *Settings, QWidget* parent) : QWidget(parent){
    settings = Settings;
}

/**
 * @brief LayoutTab::~LayoutTab
 *
 * Destructor of the LayoutTab class.
 * Deletes the GUI elements.
 */
LayoutTab::~LayoutTab(){
    delete design;
    delete highlighting;
    delete language;
}

/**
 * @brief LayoutTab::LayoutTab
 *
 * Constructor of the LayoutTab class.
 * Calls the addLayout member function.
 */
LayoutTab::LayoutTab(QHash<QString, QVariant> *Settings, QWidget* parent) : SettingsTab(Settings, parent){
    addLayout();
}

/**
 * @brief LayoutTab::addLayout
 *
 * Creates the layout tab UI and makes it interactive.
 */
void LayoutTab::addLayout(){
    design = new QGroupBox(tr("Design"));

    designBoxLabel = new QLabel(tr("Design:"));
    designBox = new QComboBox;
    QString designConfig = settings->value("Design").toString();
    for(QString style : QStyleFactory::keys()){
        designBox->addItem(style);
        if(designConfig == style)
            designBox->setCurrentIndex(designBox->count() - 1);
    }

    connect(designBox, SIGNAL(currentTextChanged(QString)), this, SLOT(designSettings(QString)));

    verticalDesign = new QHBoxLayout;
    verticalDesign->addWidget(designBoxLabel);
    verticalDesign->addWidget(designBox);

    horizontalDesign = new QVBoxLayout;
    horizontalDesign->addLayout(verticalDesign);
    design->setLayout(horizontalDesign);

    highlighting = new QGroupBox(tr("Highlighting"));

    hlBoxLabel = new QLabel(tr("Highlighting:"));
    hlBox = new QComboBox;
    hlBox->addItem(tr("GLSL"));
    hlBox->addItem(tr("Python"));
    hlBox->addItem(tr("QT"));
    hlBox->addItem(tr("Variable Names"));    
    hlBox->addItem(tr("None"));

    int hlConfig = settings->value("Highlighting").toInt();
    if(hlConfig >= 0 || hlConfig <= 4)
        hlBox->setCurrentIndex(hlConfig);

    connect(hlBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(hlSettings(int)));

    verticalHl = new QHBoxLayout;
    verticalHl->addWidget(hlBoxLabel);
    verticalHl->addWidget(hlBox);

    horizontalHl = new QVBoxLayout;
    horizontalHl->addLayout(verticalHl);
    highlighting->setLayout(horizontalHl);

    language = new QGroupBox(tr("Language"));

    languageBoxLabel = new QLabel(tr("Language:"));
    languageBox = new QComboBox;
    languageBox->addItem(tr("English"));
    languageBox->addItem(tr("German"));

    int languageConfig = settings->value("Language").toInt();
    if(languageConfig >= 0 || languageConfig <= 1)
        languageBox->setCurrentIndex(languageConfig);

    connect(languageBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(languageSettings(int)));

    verticalLanguage = new QHBoxLayout;
    verticalLanguage->addWidget(languageBoxLabel);
    verticalLanguage->addWidget(languageBox);

    horizontalLanguage = new QVBoxLayout;
    horizontalLanguage->addLayout(verticalLanguage);
    language->setLayout(horizontalLanguage);

    main = new QVBoxLayout;
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
void LayoutTab::designSettings(QString text){
    settings->insert("Design", text);
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
BehaviourTab::BehaviourTab(QHash<QString, QVariant> *Settings, QWidget* parent) : SettingsTab(Settings, parent){
    addLayout();
}

/**
 * @brief BehaviourTab::~BehaviourTab
 *
 * Destructor of the BehaviourTab class.
 * Deletes the GUI elements.
 */
BehaviourTab::~BehaviourTab(){
    delete startup;
    delete startupCompiler;
    delete compilerChoiceLabel;
    delete compilerChoice;
}

/**
 * @brief BehaviourTab::addLayout
 *
 * Creates the layout tab UI and makes it interactive.
 */
void BehaviourTab::addLayout(){
    startup = new QGroupBox(tr("Startup Behaviour"));
    openCheck = new QCheckBox(tr("Open Last Files On Startup"));
    sizeCheck = new QCheckBox(tr("Remember Size Of Application"));

    if(settings->value("OpenFiles").toBool())
        openCheck->toggle();
    else
        settings->insert("OpenFiles", true);

    if(settings->value("RememberSize").toBool())
        sizeCheck->toggle();
    else
        settings->insert("RememberSize", true);

    startupCompiler = new QButtonGroup(this);
    rememberCompiler = new QCheckBox(tr("Remember Compiler that was used last"));
    askForCompiler = new QCheckBox(tr("Always ask"));
    startupCompiler->addButton(rememberCompiler);
    startupCompiler->addButton(askForCompiler);

    if(settings->value("RememberCompiler").toBool())
        rememberCompiler->toggle();
    else{
        settings->insert("RememberCompiler", true);
        askForCompiler->toggle();
    }

    connect(openCheck, SIGNAL(toggled(bool)), this, SLOT(openSlot(bool)));
    connect(sizeCheck, SIGNAL(toggled(bool)), this, SLOT(sizeSlot(bool)));
    connect(rememberCompiler, SIGNAL(toggled(bool)), this, SLOT(rememberCompilerSlot(bool)));

    compiler = new QGroupBox(tr("Compiler to be used:"));

    compilerChoiceLabel = new QLabel(tr("Compiler:"));
    compilerChoice = new QComboBox;
    compilerChoice->addItem(tr("AudioPython"));
    compilerChoice->addItem(tr("AudioQT"));
    compilerChoice->addItem(tr("GLSL"));
    compilerChoice->addItem(tr("Python (Regular)"));

    int useCompilerConfig = settings->value("UseCompiler").toInt();
    if(useCompilerConfig >= 0 || useCompilerConfig <= 3)
        compilerChoice->setCurrentIndex(useCompilerConfig);

    connect(compilerChoice, SIGNAL(currentIndexChanged(int)), this, SLOT(useCompilerSlot(int)));

    startupLayout = new QVBoxLayout;
    startupLayout->addWidget(openCheck);
    startupLayout->addWidget(sizeCheck);
    startupLayout->addSpacing(10);
    startupLayout->addWidget(rememberCompiler);
    startupLayout->addWidget(askForCompiler);
    startup->setLayout(startupLayout);

    compilerLayout = new QVBoxLayout;
    compilerLayout->addWidget(compilerChoiceLabel);
    compilerLayout->addWidget(compilerChoice);
    compiler->setLayout(compilerLayout);

    mainLayout = new QVBoxLayout;
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
    settings->insert("OpenFiles", toggled);
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
    settings->insert("RememberSize", toggled);
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
    settings->insert("RememberCompiler", toggled);
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
