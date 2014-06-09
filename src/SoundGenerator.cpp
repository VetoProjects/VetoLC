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
    stopflag = false;
    qRegisterMetaType<QtSoundException>("QtSoundException");
    name = progName;
    instructions = qtInstructions;
    abortAction = new QAction(this);
    abortAction->setShortcut(QKeySequence("Ctrl-C"));
    connect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
}

/**
 * @brief SoundGenerator::addToOutputs
 * @param file
 * @param format
 * @return int index of the output in the Outputlist
 *
 * Creates a QAudioOutput in the desired QAudioFormat and linked
 * to the QFile specified.
 */
int SoundGenerator::addToOutputs(QFile file, QAudioFormat format){
    QAudioOutput audio(format);
    connect(&audio, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(handleStateChanged(QAudio::State)));
    audio.start(&file);
    outputs.append(&audio);
    return outputs.indexOf(&audio);
}

/**
 * @brief SoundGenerator::removeFromOutputs
 * @param index
 *
 * erases the output at index.
 */
void SoundGenerator::removeFromOutputs(int index){
    QAudioOutput* audio = outputs.at(index);
    audio->~QAudioOutput();
    outputs.removeAt(index);
}

/**
 * @brief SoundGenerator::updateCode
 * @param filename
 * @param instructions
 * @return
 *
 * STUB!
 */
bool SoundGenerator::updateCode(const QString &filename, const QString &instructions){
    return !filename.isEmpty() && !instructions.isEmpty();
}

/**
 * @brief SoundGenerator::changeOutputAt
 * @param index
 * @param file
 *
 * changes the output at index to the QFile specfied.
 */
void SoundGenerator::changeOutputAt(int index, QFile* file){
    if(file == NULL)
        throw QtSoundException("File given to changeOutputAt() was invalid: NULL");
    QAudioOutput *output = outputs.at(index);
    output->start(file);
}

/**
 * @brief SoundGenerator::run
 *
 * the main loop of the thread, running until the user interrupts or
 * the stopflag is set.
 */
void SoundGenerator::run(){
    if(instructions.isEmpty()){
        emit doneSignal(QtSoundException("File is empty. Nothing to execute."));
        return;
    }
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
