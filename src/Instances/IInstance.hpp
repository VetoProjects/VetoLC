#ifndef IINSTANCE_HPP
#define IINSTANCE_HPP

#include <QObject>
#include <QVariant>

namespace Instances{
/**
 * @brief The IInstance class
 *
 * Abstract baseclass from which all instances
 * are derived that are managed by the backend.
 */
class IInstance : public QObject{
    Q_OBJECT
public:
    const int ID;
    IInstance(int id, QObject *parent = 0) :
        QObject(parent), ID(id)
    { }
    virtual bool close() = 0;
    virtual void reportError(const QString &) = 0;
    virtual void reportWarning(const QString &) = 0;
    virtual void codeStopped() = 0;
    virtual void highlightErroredLine(int) = 0;
    virtual QString sourceCode() const = 0;
    virtual QString title() const = 0;

Q_SIGNALS:
    void runCode(IInstance *);
    void stopCode(IInstance *);

    void closing(IInstance *);
    void closeAll();

    void openHelp(IInstance *);
    void openSettings(IInstance *);

    void changeSetting(IInstance *, const QString &key, QVariant value);
    void changeSettings(IInstance *, const QHash<QString, QVariant> &);
    void getSetting(IInstance *, const QString &key, QVariant &value);
    void getSettings(IInstance *, QHash<QString, QVariant> &settings);

protected:
    IInstance& operator=(const IInstance& rhs);
    IInstance& operator=(IInstance&& rhs);
};

}

#endif // IINSTANCE_HPP
