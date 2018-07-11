#include "controls_display_2d.h"

#include "ui_controls_display_2d.h"

Controls_display_2d::Controls_display_2d(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls_display_2d)
{
    ui->setupUi(this);

    ui->cmb_colormap->addItems(display::ColorMap::getColormapList());

    connect(ui->cmb_colormap,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &Controls_display_2d::colormap_changed);

    connect(ui->chk_show_axis, &QCheckBox::stateChanged,
            this, &Controls_display_2d::show_axis);

    connect(ui->chk_show_cursor, &QCheckBox::stateChanged,
            this, &Controls_display_2d::show_cursor);
}

Controls_display_2d::~Controls_display_2d()
{
    delete ui;
}
