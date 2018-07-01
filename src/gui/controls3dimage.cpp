#include "controls3dimage.h"
#include "ui_controls3dimage.h"

Controls3DImage::Controls3DImage(QWidget *parent) :
    Controls2DImage(parent),
    ui(new Ui::Controls3DImage)
{
    ui->setupUi(this);
}

Controls3DImage::~Controls3DImage()
{
    delete ui;
}
