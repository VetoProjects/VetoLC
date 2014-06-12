#include "PyLiveInterpreter.hpp"

/**
 * @brief PyLiveInterpreter::PyLiveInterpreter
 * @param progName
 * @param pyInstructions
 *
 * The constructor of the PyLiveInterpreterclass.
 * Sets up the python interpreter, the instructions with
 * which it will be fed, the class variables and the break shortcut.
 */
PyLiveInterpreter::PyLiveInterpreter(char* progName, char* pyInstructions){
    if(pyInstructions == QString("")){
        emit doneSignal(PythonException("File is empty. Nothing to execute."));
        return;
    }
    qRegisterMetaType<PythonException>("PythonException");

    Py_SetProgramName(progName);
    Py_Initialize();
    instructions = pyInstructions;
    ownExcept = PythonException("Script terminated successfully.");
    abortAction = new QAction(this);
    abortAction->setShortcut(QKeySequence("Ctrl-C"));
    connect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
    PyObject* module = PyImport_AddModule("__main__");
    dict = PyModule_GetDict(module);
}

/**
 * @brief PyLiveInterpreter::~PyLiveInterpreter
 *
 * The destructor of the PyLiveInterpreter class.
 * Kills the python interpreter.
 */
PyLiveInterpreter::~PyLiveInterpreter(){
    delete abortAction;
    Py_Finalize();
}

/**
 * @brief PyLiveInterpreter::run
 *
 * The main loop. Calls the user code executing method
 * and emits a signal when it's done.
 */
void PyLiveInterpreter::run(){
    PyObject* check = execute();
    if(check == NULL)
        exceptionOccurred();
    emit doneSignal(ownExcept);
}

/**
 * @brief PyLiveInterpreter::loop
 * @return PyObject* / NULL if there was an exception
 *         in the python interpreter.
 *
 * executes the python code in the interpreter.
 */
PyObject* PyLiveInterpreter::execute(){
    return PyRun_String(instructions.toLocal8Bit().data(),
                        Py_file_input, dict, dict);
}

/**
 * @brief PyLiveInterpreter::updateCode
 * @param filename
 * @param instructions
 * @return true if the name of the program could be changed,
 *         false otherwise
 *
 * Updates the code of the currently running Python interpreter.
 */
bool PyLiveInterpreter::updateCode(QString filename, QString instructions){
    Py_SetProgramName(filename.toLocal8Bit().data());
    this->instructions = instructions;
    return true;
}

/**
 * @brief PyLiveInterpreter::exceptionOccurred
 * @return PythonException
 *
 * Fetches the Python Exception and translates it to a C++
 * exception.
 */
void PyLiveInterpreter::exceptionOccurred(){
    PyObject *errtype, *errvalue, *traceback;
    PyErr_Fetch(&errtype, &errvalue, &traceback);
    PyErr_NormalizeException(&errtype, &errvalue, &traceback);
    QString exceptionText = QString(PyString_AsString(PyObject_Str(errtype)));
    exceptionText.append(": ");
    exceptionText.append(PyString_AsString(PyObject_Str(errvalue)));
    ownExcept = PythonException(exceptionText);
}

/**
 * @brief PyLiveInterpreter::terminated
 *
 * SLOT that is called when the user interrupt(CTRL-C) SIGNAL
 * is emitted.
 */
void PyLiveInterpreter::terminated(){
    ownExcept = PythonException("User Terminated.");
}
