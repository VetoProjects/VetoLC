#ifndef QTGLEXCEPTION
#define QTGLEXCEPTION

#include <QException>

/**
 * @brief The QtGlException class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QException that is used for communicating
 * the status of the opengl interpreter.
 */
class QtGlException : public QException{
public:
    QtGlException() :
        QException()
    {}
    QtGlException(const char* message){
        exceptionText = message;
    }
    //~QtGlException(){}
    virtual const char* what() const throw(){
        return exceptionText;
    }
private:
    const char* exceptionText;
};

#endif // QTGLEXCEPTION
