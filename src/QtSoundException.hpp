#ifndef QTSOUNDEXCEPTION
#define QTSOUNDEXCEPTION

#include <QString>
#include <QException>

/**
 * @brief The pythonException class
 * @author Veit Heller(s0539501) & Tobias Brosge(s0539713)
 *
 * A subclass of QException that is used for communicating
 * the status of the sound generator.
 */
class QtSoundException : public QException{
public:
    QtSoundException(){}
    QtSoundException(const QString &message){
        exceptionText = message;
    }

    ~QtSoundException() throw() {}

    virtual const char* what() const throw(){
        return exceptionText.toLocal8Bit().data();
    }
private:
    QString exceptionText;
};

#endif // QTSOUNDEXCEPTION
