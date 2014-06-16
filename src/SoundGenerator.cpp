#include "SoundGenerator.hpp"

/**
 * @brief SoundGenerator::SoundGenerator
 * @param progName
 * @param qtInstructions
 *
 * Constructor of the SoundGenerator class.
 * Sets up QT, the class variables, the instructions
 * and the break SIGNAL.
 */
SoundGenerator::SoundGenerator(const QString &progName, const QString &qtInstructions){
    qRegisterMetaType<QtSoundException>("QtSoundException");
    if(qtInstructions == ""){
        emit doneSignal(QtSoundException("File is empty. Nothing to execute."));
        return;
    }
    //engine = new QQmlEngine(this);
    name = progName;
    instructions = qtInstructions;
    abortAction = new QAction(this);
    abortAction->setShortcut(QKeySequence("Ctrl-C"));
    connect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
    emit doneSignal(QtSoundException("Terminated successfully."));
}

/**
 * @brief SoundGenerator::updateCode
 * @param filename
 * @param instructions
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
            if(stopflag)
                throw QtSoundException("Terminated successfully.");
        }
    }catch(QtSoundException e){
        ownExcept = e;
    }catch(std::exception& e){
        ownExcept = QtSoundException(e.what());
    }
    delete abortAction;
    emit doneSignal(ownExcept);
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
 * is emitted.
 */
void SoundGenerator::terminated(){
    stopflag = true;
}
