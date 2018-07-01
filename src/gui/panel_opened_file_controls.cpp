#include "panel_opened_file_controls.h"
#include "ui_panel_opened_file_controls.h"

Panel_opened_file_controls::Panel_opened_file_controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_file_controls)
{
    ui->setupUi(this);

    ctrl_2D = new Controls2DImage(this);
    ctrl_2D->setVisible(false);
    ui->horizontalLayout->addWidget(ctrl_2D);

    ctrl_3D = new Controls3DImage(this);
    ctrl_3D->setVisible(false);
    ui->horizontalLayout->addWidget(ctrl_3D);
}

Panel_opened_file_controls::~Panel_opened_file_controls()
{
    delete ui;
}


void Panel_opened_file_controls::show_panel(int dim)
{
    switch (dim) {
    case 0:
        ctrl_2D->setVisible(false);
        ctrl_3D->setVisible(false);
        return;
    case 2:
        ctrl_2D->setVisible(true);
        ctrl_3D->setVisible(false);
        return;
    case 3:
        ctrl_2D->setVisible(false);
        ctrl_3D->setVisible(true);
    default:
        ctrl_2D->setVisible(false);
        ctrl_3D->setVisible(false);
        break;
    }
}
