#include "controls2dimage.h"
#include "ui_controls2dimage.h"

Controls2DImage::Controls2DImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls2DImage)
{
    ui->setupUi(this);
}

Controls2DImage::~Controls2DImage()
{
    delete ui;
}
