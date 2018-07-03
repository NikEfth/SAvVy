#include "controls_display_2d.h"

#include "ui_controls_display_2d.h"

Controls_display_2d::Controls_display_2d(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls_display_2d)
{
    ui->setupUi(this);

    ui->cmb_colormap->addItems(display::ColorMap::getColormapList());
}

Controls_display_2d::~Controls_display_2d()
{
    delete ui;
}
