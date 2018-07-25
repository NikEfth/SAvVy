#ifndef SAVVY_INTERFACE_H
#define SAVVY_INTERFACE_H

#define ExternalInterface_iid "org.Savvy.ExternalInterface"

#include <QtPlugin>
#include <QDialog>

class ExternalInterface : public QDialog
{
public:
    inline explicit ExternalInterface(QWidget *parent = nullptr) :
        QDialog(parent)
    {

    }

    inline virtual ~ExternalInterface()
    {

    }

    inline virtual void load_from_workspace() = delete;

    inline virtual void return_to_workspace() = delete;

    inline QString & get_name()
    {
        return m_name;
    }

private:
    QString m_name;
};

Q_DECLARE_INTERFACE(ExternalInterface,ExternalInterface_iid)

#endif //SAVVY_INTERFACE_H
