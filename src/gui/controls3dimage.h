#ifndef CONTROLS3DIMAGE_H
#define CONTROLS3DIMAGE_H

#include <QWidget>

namespace Ui {
class Controls3DImage;
}

class Controls3DImage : public QWidget
{
    Q_OBJECT

public:
    explicit Controls3DImage(QWidget *parent = 0);
    ~Controls3DImage();

private:
    Ui::Controls3DImage *ui;
};

#endif // CONTROLS3DIMAGE_H
