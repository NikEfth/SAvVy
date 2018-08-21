#include "controls_display_2d.h"

#include "ui_controls_display_2d.h"

Controls_display_2d::Controls_display_2d(const QStringList &colorMapNames,
                                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls_display_2d)
{
    ui->setupUi(this);

    ui->cmb_colormap->addItems(colorMapNames);

    connect(ui->cmb_colormap,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &Controls_display_2d::colormap_changed);
}

Controls_display_2d::~Controls_display_2d()
{
    delete ui;
}
