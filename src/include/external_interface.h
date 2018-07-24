#ifndef SAVVY_INTERFACE_H
#define SAVVY_INTERFACE_H

#include <QtPlugin>
#include <QDialog>
#include <memory>

#include "src/gui/workspace.h"

class ExternalInterface : public QDialog
{
public:
    explicit ExternalInterface(QWidget *parent = 0) :
        QDialog(parent)
    {}

    virtual ~ExternalInterface() {}

    virtual void load_from_workspace(std::shared_ptr<Workspace> ) = 0;

    virtual void return_to_workspace(std::shared_ptr<Workspace> ) = 0;

    virtual void show_workspace_operations(bool state) = 0;

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
