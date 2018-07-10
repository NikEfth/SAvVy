#include "guitests.h"
#include <QMessageBox>

GuiTests::GuiTests(QObject *parent) : QObject(parent)
{

}

int GuiTests::ask(QString question)
{
    QMessageBox msgBox;
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}
