#include "panel_opened_file_controls.h"
#include "ui_panel_opened_file_controls.h"

Panel_opened_file_controls::Panel_opened_file_controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_file_controls)
{
    ui->setupUi(this);

    controls_1d = new Controls_display_1d(this);
    controls_2d = new Controls_display_2d(this);

    controls_1d->setHidden(true);
    controls_2d->setHidden(true);

    ui->verticalLayout_2->addWidget(controls_1d);
    ui->verticalLayout_2->addWidget(controls_2d);
}

Panel_opened_file_controls::~Panel_opened_file_controls()
{
    delete ui;
}


void Panel_opened_file_controls::show_panel(int dim)
{
    controls_1d->setHidden(true);
    controls_2d->setHidden(true);

    switch (dim) {
    case 1:
        controls_1d->setHidden(false);
        break;
    case 2:
        controls_2d->setHidden(false);
        break;
    case 3:

        break;
    default:
        break;
    }
}
