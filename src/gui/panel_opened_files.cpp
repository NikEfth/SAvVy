#include "panel_opened_files.h"
#include "ui_panel_opened_files.h"

Panel_opened_files::Panel_opened_files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_files)
{
    ui->setupUi(this);
}

Panel_opened_files::~Panel_opened_files()
{
    delete ui;
}
