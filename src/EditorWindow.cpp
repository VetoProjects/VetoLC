#include "EditorWindow.hpp"

/**
 * @brief EditorWindow::EditorWindow
 *
 * The constructor of the EditorWindow class.
 * Sets up the whole UI around the Editor,
 * including slots and signals.
 * Also loads the settings and loads the file
 * that was last displayed when closing the editor.
 * Also, it deals with platform-specific displaying quirks.
 */
EditorWindow::EditorWindow(const QHash<QString, QVariant> &settings, QWidget *parent) : QMainWindow(parent){
    codeEditor = new CodeEditor(this, settings.value("UseCompiler").toInt());
    setCentralWidget(codeEditor);

    addActions();
    addMenus();
    addToolBars();
    addStatusBar();

    connect(codeEditor->document(), SIGNAL(contentsChanged()), this, SLOT(docModified()));

    templateNum = settings.value("UseCompiler").toInt();
    applySettings(settings);

    // Mac quirks
    setUnifiedTitleAndToolBarOnMac(true);
}

/**
 * @brief EditorWindow::~EditorWindow
 *
 * Destructor of the EditorWindow class.
 * Deletes all the GUI elements.
 */
EditorWindow::~EditorWindow(){
    delete codeEditor;
    delete newAction;
    delete openAction;
    delete saveAction;
    delete saveAsAction;
    delete exitAction;
    delete runAction;
    delete settingsAction;
    delete helpAction;
    delete fMenu;
    delete eMenu;
    delete hMenu;
    delete fileBar;
    delete editBar;
}

/**
 * @brief EditorWindow::closeEvent
 * @param event
 *
 * reacts to the close signal; saves the current preferences
 * if wanted and exits(SLOT).
 */
void EditorWindow::closeEvent(QCloseEvent *event){
    if(saveDialog()){
        saveSettings();
        emit closing(this);
        event->accept();
    }else
        event->ignore();
}

void EditorWindow::gotOpenHelp()
{
    emit openHelp(this);
}

void EditorWindow::gotCloseAll()
{
    emit closeAll(this);
}

/**
 * @brief EditorWindow::newFile
 *
 * opens a new, empty file in the editor(SLOT).
 */
void EditorWindow::newFile(){
    if(saveDialog()){
        codeEditor->clear();
        setAsCurrentFile("");
        if(templateNum == 0)
            loadFile(":/rc/template.py");
        else if(templateNum == 2)
            loadFile(":/rc/template.glsl");
    }
}

/**
 * @brief EditorWindow::openFile
 *
 * Opens a file choosing dialog and opens
 * the requested file(SLOT).
 */
void EditorWindow::openFile(){
    if(saveDialog()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()){
            if(fileName.endsWith("py")){
                templateNum = 0;
                codeEditor->setHighlighting(0);
                emit changedSetting(this, "UseCompiler", 0);
            }else if(fileName.endsWith("qml")){
                templateNum = 1;
                codeEditor->setHighlighting(1);
                emit changedSetting(this, "UseCompiler", 1);
            }else if(fileName.endsWith("glsl")){
                templateNum = 2;
                codeEditor->setHighlighting(2);
                emit changedSetting(this, "UseCompiler", 2);
            }
            loadFile(fileName);
        }
    }
}

/**
 * @brief EditorWindow::saveFile
 * @return the return codes of the
 *         saveFileAs()/saveFile(filename) functions
 *
 * reacts to the saveFile SIGNAL and redirects to
 * either saveFileAs() (if there is no filename assgined
 * to the current file) or saveFile(filename) (if there is
 * a name to the current file)(SLOT).
 */
bool EditorWindow::saveFile(){
   if(currentFile.isEmpty()){
        return saveFileAs();
    }else{
        return saveFile(currentFile);
    }
}

/**
 * @brief EditorWindow::saveFileAs
 * @return bool - false if the file choosing dialog
 *         failed/return code of the saveFile(filename)
 *         function otherwise
 *
 * opens a file choosing dialog and saves the contents
 * of the text editor under the chosen name(SLOT).
 */
bool EditorWindow::saveFileAs(){
    QString fileName = QFileDialog::getSaveFileName(this);
    return !fileName.isEmpty() && saveFile(fileName);
}

/**
 * @brief EditorWindow::saveSettings
 *
 * saves own settings.
 */
void EditorWindow::saveSettings(){
    QHash<QString, QVariant> settings;
    settings.insert("pos", this->pos());
    settings.insert("size", this->size());
    if(!currentFile.contains("template."))
        settings.insert("file", currentFile);
    else
        settings.insert("file", "");
    emit changedSettings(this, settings);
}

/**
 * @brief EditorWindow::openSettings
 *
 * creates an instance of SettingsWindow and executes it.
 */
void EditorWindow::gotOpenSettings(){
    emit openSettings(this);
}

/**
 * @brief EditorWindow::docModified
 *
 * reacts to the contentsChanged() SIGNAL by setting the
 * window properties to modified/not modified( as shown by
 * the little * at the top of the window beside the name).
 */
void EditorWindow::docModified(){
    setWindowModified(codeEditor->document()->isModified());
}

/**
 * @brief EditorWindow::warningDisplay
 * @param message
 *
 * Displays a warning box containing message.
 */
void EditorWindow::warningDisplay(const QString &message){
    QMessageBox::warning(this, tr("VeToLC"), message);
}

void EditorWindow::codeStopped()
{
    runAction->setIcon(QIcon(":/images/run.png"));
}

/**
 * @brief EditorWindow::apply‚Settings
 *
 * Reads the "persistent platform-independent
 * application settings" and gets the settings.
 */
void EditorWindow::applySettings(const QHash<QString, QVariant> &settings){
    if(settings.value("RememberSize").toBool()){
        const QPoint  pos  = settings.value("pos", QPoint(200,200)).toPoint();
        const QSize   size = settings.value("size", QSize(600, 600)).toSize();
        move(pos);
        resize(size);
    }
    if(settings.value("OpenFiles").toBool()){
        const QString file = settings.value("file", "").toString();
        if(file.isEmpty()){
            if(templateNum == 0)
                loadFile(":/rc/template.py");
            else if(templateNum == 2)
                loadFile(":/rc/template.glsl");
            setAsCurrentFile("");
        }
        else
            loadFile(file);
    }
}

/**
 * @brief EditorWindow:: runFile
 *
 * lets the backend run the file.
 */
void EditorWindow::runFile(){
    runAction->setIcon(QIcon(":/images/refresh.png"));
    runCode(this);
}

/**
 * @brief EditorWindow::showResults
 * @param returnedValue
 *
 * Is called after the execution of the interpreter thread has finished.
 * Shows its return code or an exception traceback.
 */
void EditorWindow::showResults(const QString &returnedValue){
    statusBar()->showMessage(returnedValue, 5000);
    runAction->setIcon(QIcon(":/images/run.png"));
}

/**
 * @brief EditorWindow::getSourceCode
 * @return plain text in editor
 *
 * returns code in editor.
 */
QString EditorWindow::getSourceCode() const{
    return codeEditor->toPlainText();
}

/**
 * @brief EditorWindow::getTitle
 * @return current file name as string.
 *
 * returns current file name.
 */
QString EditorWindow::getTitle() const{
    return currentFile;
}

/**
 * @brief EditorWindow::addActions
 *
 * adds signals(buttons, shortcuts) to the window.
 */
void EditorWindow::addActions(){
    newAction = new QAction(QIcon(":/images/new.png"), tr("&New File"), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the document to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(gotCloseAll()));

    runAction = new QAction(QIcon(":/images/run.png"), tr("&Run"), this);
    runAction->setShortcuts(QKeySequence::Refresh);
    runAction->setStatusTip(tr("Runs the code in the editor"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(runFile()));

    settingsAction = new QAction(QIcon(":/images/settings.png"), tr("Settings"), this);
    settingsAction->setShortcuts(QKeySequence::Preferences);
    settingsAction->setStatusTip(tr("Opens A Settings Window"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(gotOpenSettings()));

    helpAction = new QAction(tr("About"), this);
    helpAction->setShortcuts(QKeySequence::HelpContents);
    helpAction->setStatusTip(tr("Opens the Help"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(gotOpenHelp()));
}

/**
 * @brief EditorWindow::addMenus
 *
 * adds the menus to the window(duh!) and fills them
 * with the corresponding actions.
 */
void EditorWindow::addMenus(){
    fMenu = menuBar()->addMenu(tr("&File"));
    fMenu->addAction(newAction);
    fMenu->addAction(openAction);
    fMenu->addAction(saveAction);
    fMenu->addAction(saveAsAction);
    fMenu->addSeparator();
    fMenu->addAction(exitAction);

    eMenu = menuBar()->addMenu(tr("&Edit"));
    eMenu->addAction(settingsAction);
    eMenu->addAction(runAction);
    menuBar()->addSeparator();

    hMenu = menuBar()->addMenu(tr("&Help"));
    hMenu->addAction(helpAction);
}

/**
 * @brief EditorWindow::addToolBars
 *
 * adds the toolbars to the window(duh!) and fills them
 * with the corresponding actions.
 */
void EditorWindow::addToolBars(){
    fileBar = addToolBar(tr("File"));
    fileBar->addAction(newAction);
    fileBar->addAction(openAction);
    fileBar->addAction(saveAction);

    editBar = addToolBar(tr("Edit"));
    fileBar->addAction(settingsAction);
    fileBar->addAction(runAction);
}

/**
 * @brief EditorWindow::addStatusBar
 *
 * Adds a status bar to the window(duh!) and shows a
 * message in it displaying it is ready to use.
 */
void EditorWindow::addStatusBar(){
    statusBar()->showMessage(tr("Ready"));
}

/**
 * @brief EditorWindow::saveDialog
 * @return bool - true if the user does not want to save or the
 *         document is saved in its current state/false if he
 *         cancels/return code of the saveFile() function
 *
 * Opens a dialog asking the user if he wants to save the changes.
 * Invoked when he exits the application/the current file unchanged.
 */
bool EditorWindow::saveDialog(){
    if(codeEditor->document()->isModified()){
        QMessageBox::StandardButton question;
        question = QMessageBox::warning(this, tr("VeToLC"),
                                tr("The document has been modified"
                                   " but is unsaved.\n"
                                "Do you want to save your changes?"),
                                QMessageBox::Save | QMessageBox::Discard
                                | QMessageBox::Cancel);
        if(question == QMessageBox::Save)
            return saveFile();
        else if(question == QMessageBox::Cancel)
            return false;
    }
    return true;
}

/**
 * @brief EditorWindow::loadFile
 * @param path
 *
 * loads a file of a given name
 */
void EditorWindow::loadFile(const QString &path){
    QFile file(path);
    //display an error message if the file cannot be opened and why
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("VeToLC"),
                             tr("Cannot read file %1:\n%2.")
                              .arg(path)
                              .arg(file.errorString()));
        return;
    }

    /*
     * while the application is busy loading,
     * set a waiting cursor. Real fancy, huh?
     */
    QTextStream in(&file);
#ifdef QT_NO_CURSOR
    codeEditor->setPlainText(in.readAll());
#else
    QApplication::setOverrideCursor(Qt::WaitCursor);
    codeEditor->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
#endif

    //set current file and display a message in the status bar
    setAsCurrentFile(path);
    statusBar()->showMessage(tr("File was loaded"), 2000);
    emit changedSetting(this, "file", path);
}

/**
 * @brief EditorWindow::saveFile
 * @param name
 * @return bool - false if the file cannot be saved/
 *         true if everything went fine
 *
 * saves the current file under a provided name
 */
bool EditorWindow::saveFile(const QString &name){
    QFile file(name);
    //display an error message if the file cannot be saved and why
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        warningDisplay(tr("Cannot write file %1:\n%2.")
                           .arg(name)
                           .arg(file.errorString()));
        return false;
    }

    /*
     * while the application is busy loading,
     * set a waiting cursor. Real fancy, huh?
     */
    QTextStream out(&file);
#ifdef QT_NO_CURSOR
    out << codeEditor->toPlainText();
#else
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << codeEditor->toPlainText();
    QApplication::restoreOverrideCursor();
#endif

    //set current file and display a message in the status bar
    setAsCurrentFile(name);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}


/**
 * @brief EditorWindow::setAsCurrentFile
 * @param name
 *
 * sets the file name and displays it at the top of the window
 */
void EditorWindow::setAsCurrentFile(const QString &name){
    currentFile = name;
    codeEditor->document()->setModified(false);

    setWindowModified(false);

    setWindowFilePath(currentFile);
    if(currentFile != "" && !currentFile.contains("template."))
        setWindowTitle("VeToLC | " + stripName(currentFile) + "[*]");
    else
        setWindowTitle("VeToLC [*]");
}

/**
 * @brief EditorWindow::stripName
 * @param fullName
 * @return return code of QFileInfo
 *
 * strips the fully qualified name of a file to the unqualified one
 */
QString EditorWindow::stripName(const QString &fullName){
    return QFileInfo(fullName).fileName();
}
