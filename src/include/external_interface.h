#ifndef SAVVY_INTERFACE_H
#define SAVVY_INTERFACE_H

#include <QtPlugin>
#include <QDialog>

class ExternalInterface : public QDialog
{
public:
    virtual ~ExternalInterface() {}

    virtual void load_from_workspace() = 0;

    virtual void return_to_workspace() = 0;

    QString get_name()
    {
        return _name;
    }
protected:
    QString _name;
};

#define ExternalInterface_iid "org.Savvy.ExternalInterface"

Q_DECLARE_INTERFACE(ExternalInterface,ExternalInterface_iid)


#endif
