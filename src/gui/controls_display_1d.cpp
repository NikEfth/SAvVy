#include "controls_display_1d.h"
#include "ui_controls_display_1d.h"

Controls_display_1d::Controls_display_1d(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls_display_1d)
{
    ui->setupUi(this);
}

Controls_display_1d::~Controls_display_1d()
{
    delete ui;
}
