#include "savvywindow.h"
#include "ui_savvywindow.h"

SavvyWindow::SavvyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SavvyWindow)
{
    ui->setupUi(this);
}

SavvyWindow::~SavvyWindow()
{
    delete ui;
}
