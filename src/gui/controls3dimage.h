#ifndef CONTROLS3DIMAGE_H
#define CONTROLS3DIMAGE_H

#include <QWidget>
#include "controls2dimage.h"

namespace Ui {
class Controls3DImage;
}

class Controls3DImage : public Controls2DImage
{
    Q_OBJECT

public:
    explicit Controls3DImage(QWidget *parent = 0);
    ~Controls3DImage();

private:
    Ui::Controls3DImage *ui;
};

#endif // CONTROLS3DIMAGE_H
