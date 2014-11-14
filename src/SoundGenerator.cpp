#include "SoundGenerator.hpp"

/**
 * @brief SoundGenerator::SoundGenerator
 * @param progName
 * @param qtInstructions
 *
 * Constructor of the SoundGenerator class.
 * Sets up the QML interpreter, the class variables,
 * the instructions and the break SIGNAL.
 */
SoundGenerator::SoundGenerator(const QString &progName, const QString &qtInstructions){
    if(qtInstructions == ""){
        Q_EMIT doneSignal(tr("File is empty. Nothing to execute."));
        return;
    }
//    engine = new QQmlEngine(this);
    name = progName;
    instructions = qtInstructions;
    abortAction = new QAction(this);
    abortAction->setShortcut(QKeySequence("Ctrl-C"));
    connect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
    Q_EMIT doneSignal(tr("Terminated successfully."));
}

/**
 * @brief SoundGenerator::updateCode
 * @param filename
 * @param qtInstructions
 * @return true when name and instructions are valid, false otherwise
 *
 *
 */
bool SoundGenerator::updateCode(const QString &filename, const QString &qtInstructions){
    if(!qtInstructions.isEmpty()){
        name = filename;
        instructions = qtInstructions;
        return true;
    }
    return false;
}

/**
 * @brief SoundGenerator::run
 *
 * the main loop of the thread, running until the user interrupts or
 * the stopflag is set.
 */
void SoundGenerator::run(){
    try{
        while(true){
            loop();
            if(stopflag){
                ownExcept = tr("Terminated successfully.");
                break;
            }
        }
    }catch(std::exception& e){
        ownExcept = e.what();
    }
     disconnect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
    delete abortAction;
    Q_EMIT doneSignal(ownExcept);
}

/**
 * @brief SoundGenerator::loop
 *
 * The stub that will be rewritten at compile time.
 */
void SoundGenerator::loop(){
    terminated();
}

/**
 * @brief SoundGenerator::terminated
 *
 * SLOT that is called when the user interrupt(CTRL-C) SIGNAL
 * is Q_EMITted.
 */
void SoundGenerator::terminated(){
    stopflag = true;
}
