#include "display_manager.h"
#include "ui_display_manager.h"

#include "display_container_1d.h"
#include "display_container_2d.h"
#include "display_container_3d.h"

Display_manager::Display_manager(int _my_id, int _num_dim, QWidget *parent) :
    DisplayInterface(_my_id, _num_dim, parent),
    ui(new Ui::Display_manager)
{
    ui->setupUi(this);

    switch (dims) {
    case 1:
    {
        _display = new Display_container_1d(1, this);
        set_no_controls(true);
    }
        break;
    case 2:
    {
        _display = new Display_container_2d(2, this);
        set_no_controls(true);
    }
        break;
    case 3:
    {
        _display = new Display_container_3d(3, this);
        connect(ui->sld_plane, &QSlider::sliderMoved, _display, &Display_container::update_scene);
        //        connect(ui->sld_plane, &QSlider::sliderMoved, this, &Display_manager::updated_display);
        connect(_display, &Display_container::setup_ready, this, &Display_manager::initialised_display);
    }
        break;
    }

    ui->verticalLayout_2->addWidget(_display);
}

Display_manager::Display_manager(int _my_id, stir::ArrayInterface *__display, QWidget *parent) :
    DisplayInterface(_my_id, __display->get_num_dimensions(),  parent),
    ui(new Ui::Display_manager)
{
    ui->setupUi(this);

    switch (dims) {
    case 1:
    {
        stir::Array<1, float>* tmp = dynamic_cast<stir::Array<1, float>* >(__display);
        _display = new Display_container_1d(*tmp, static_cast<int>(tmp->size()), 1, this);
        set_no_controls(true);
    }
        break;
    case 2:
    {
        stir::Array<2, float>* tmp = dynamic_cast<stir::Array<2, float>* >(__display);
        _display = new Display_container_2d(*tmp, 2, this);
         set_no_controls(true);
    }
        break;
    case 3:
    {
        stir::Array<3, float>* tmp = dynamic_cast<stir::Array<3, float>* >(__display);
        _display = new Display_container_3d(*tmp, 3, this);

        connect(ui->sld_plane, &QSlider::sliderMoved, _display, &Display_container::update_scene);
        connect(ui->sld_plane, &QSlider::sliderMoved, this, &Display_manager::updated_display);
        connect(_display, &Display_container::setup_ready, this, &Display_manager::initialised_display);
        initialised_display();
    }
        break;
    }

    ui->verticalLayout_2->addWidget(_display);
}

Display_manager::~Display_manager()
{
    delete _display;
    delete ui;
}

void Display_manager::closeEvent(QCloseEvent *event)
{
    // to silence warning
    if(event)
    {

    }

    emit aboutToClose();
}

void Display_manager::updated_display(int position)
{
    ui->lbl_plane->setText(QString::number(position) + " / " + QString::number(_display->get_num_data()));
}

void Display_manager::set_no_controls(bool _f)
{
    ui->lbl_plane->setHidden(_f);
    ui->sld_plane->setHidden(_f);
}

void Display_manager::initialised_display()
{
    ui->sld_plane->setMaximum(_display->get_num_data()-1);
    int val = ui->sld_plane->value();
    ui->lbl_plane->setText(get_label(val, _display->get_num_data()));
}
