#ifndef PYTHONEXCEPTION_HPP
#define PYTHONEXCEPTION_HPP

#include <exception>
#include <QString>

/**
 * @brief The PythonException class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 * @version 0.1.0
 *
 * A subclass of std::exception that is used for communicating
 * the status of the sound generator.
 */
class PythonException : public std::exception{
public:
    PythonException(){
    }

    PythonException(QString message){
        exceptionText = message;
    }

    ~PythonException(){
    }

    virtual const char* what() const throw(){
        return exceptionText.toLocal8Bit().data();
    }
private:
    QString exceptionText;
};

#endif // PYTHONEXCEPTION_HPP
