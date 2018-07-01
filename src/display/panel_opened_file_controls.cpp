#include "panel_opened_file_controls.h"
#include "ui_panel_opened_file_controls.h"

Panel_opened_file_controls::Panel_opened_file_controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_file_controls)
{
    ui->setupUi(this);
}

Panel_opened_file_controls::~Panel_opened_file_controls()
{
    delete ui;
}
