#include "display_manager.h"
#include "ui_display_manager.h"

#include "display_container_1d.h"
#include "display_container_2d.h"
#include "display_container_3d.h"

Display_manager::Display_manager(int _my_id, int _num_dim, QWidget *parent) :
    QWidget(parent), DisplayInterface(_my_id, _num_dim),
    ui(new Ui::Display_manager)
{
    ui->setupUi(this);

//    switch (dims) {
//    case 1:
//        _display = new Display_container_1d(my_id,1, this);
//        break;
//    case 2:
//        _display = new Display_container_2d(my_id, 2, this);
//        break;
//    case 3:
        _display = new Display_container_3d(my_id, 3, this);
//        break;
//    default:
//        break;
//    }

    ui->verticalLayout_2->addWidget(_display);

    connect(ui->sld_plane, &QSlider::sliderMoved, _display, &Display_container::update_scene);
    connect(ui->sld_plane, &QSlider::sliderMoved, this, &Display_manager::updated_display);
    connect(_display, &Display_container::setup_ready, this, &Display_manager::initialised_display);
}

Display_manager::~Display_manager()
{
    delete ui;
}

void Display_manager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose();
}

void Display_manager::updated_display(int position)
{
    ui->lbl_plane->setText(QString::number(position) + " / " + QString::number(_display->get_num_planes()));
}

void Display_manager::initialised_display()
{
    ui->sld_plane->setMaximum(_display->get_num_planes()-1);
    int val = ui->sld_plane->value();
    ui->lbl_plane->setText(get_label(val, _display->get_num_planes()));
}
