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
    if(pyInstructions == QString()){
        Q_EMIT doneSignal(tr("File is empty. Nothing to execute."), -1);
        return;
    }

//    Py_SetProgramName(progName);
    Py_Initialize();
    instructions = pyInstructions;
    ownExcept = tr("Script terminated successfully.");
    abortAction = new QAction(this);
    abortAction->setShortcut(QKeySequence("Ctrl-C"));
    connect(abortAction, SIGNAL(triggered()), this, SLOT(terminated()));
    PyObject* module = PyImport_AddModule("__main__");
    dict = PyModule_GetDict(module);
    exceptNum = -1;
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
 * and Q_EMITs a signal when it's done.
 */
void PyLiveInterpreter::run(){
    PyObject* check = execute();
    if(check == NULL)
        exceptionOccurred();
    Q_EMIT doneSignal(ownExcept, exceptNum);
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
//    Py_SetProgramName(filename.toLocal8Bit().data());
//    this->instructions = instructions;
    return true;
}

/**
 * @brief PyLiveInterpreter::exceptionOccurred
 * @return PythonException
 *
 * Fetches the Python Exception and translates it to a QString
 * and an int representing exception and line number.
 */
void PyLiveInterpreter::exceptionOccurred(){
//    PyObject *errtype, *errvalue, *traceback;
//    PyObject *mod;
//    PyObject *ret, *list, *string;
//    PyErr_Fetch(&errtype, &errvalue, &traceback);
//    PyErr_NormalizeException(&errtype, &errvalue, &traceback);
//    QString exceptionText = QString(PyString_AsString(PyObject_Str(errtype)));
//    exceptionText.append(": '");
//    exceptionText.append(PyString_AsString(PyObject_Str(errvalue)));
//    QRegExp line("line [0-9]+");
//    if(line.indexIn(exceptionText) != -1){
//        QString text = line.capturedTexts().at(0);
//        exceptionText.replace(" (<string>, " + text + ")", "");
//        exceptionText.append("' at " + text);
//        text.replace("line ", "");
//        exceptNum = text.toInt();
//    } else{
//        mod = PyImport_ImportModule("traceback");
//        list = PyObject_CallMethod(mod, (char*)"format_exception",
//                               (char*)"OOO", errtype, errvalue, traceback);
//        if(list == 0){
//            ownExcept = exceptionText;
//            exceptNum = -1;
//            return;
//        }
//        string = PyString_FromString("\n");
//        ret = _PyString_Join(string, list);
//        if(line.indexIn(PyString_AsString(ret)) != -1){
//            QString text = line.capturedTexts().at(0);
//            exceptionText.append("' at " + text);
//            text.replace("line ", "");
//            exceptNum = text.toInt();
//        } else{
//            exceptNum = -1;
//        }
//        Py_DECREF(list);
//        Py_DECREF(string);
//        Py_DECREF(ret);
//    }
//    PyErr_Clear();
//    ownExcept = exceptionText;
}

/**
 * @brief PyLiveInterpreter::terminated
 *
 * SLOT that is called when the user interrupt(CTRL-C) SIGNAL
 * is Q_EMITted.
 */
void PyLiveInterpreter::terminated(){
    ownExcept = tr("User Terminated.");
}
