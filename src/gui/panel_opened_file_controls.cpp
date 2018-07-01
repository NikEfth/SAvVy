#include "panel_opened_file_controls.h"
#include "ui_panel_opened_file_controls.h"

Panel_opened_file_controls::Panel_opened_file_controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_file_controls)
{
    ui->setupUi(this);

    ctrl_3D = new Controls3DImage(this);
    ctrl_3D->setVisible(false);
    ui->verticalLayout_2->addWidget(ctrl_3D);
}

Panel_opened_file_controls::~Panel_opened_file_controls()
{
    delete ui;
}


void Panel_opened_file_controls::show_panel(int dim)
{
    switch (dim) {
    case 0:
        ctrl_3D->setVisible(false);
        return;
    case 3:
        ctrl_3D->setVisible(true);
    default:
        ctrl_3D->setVisible(false);
        break;
    }
}
