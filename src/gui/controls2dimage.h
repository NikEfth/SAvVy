#ifndef CONTROLS2DIMAGE_H
#define CONTROLS2DIMAGE_H

#include <QWidget>

namespace Ui {
class Controls2DImage;
}

class Controls2DImage : public QWidget
{
    Q_OBJECT

public:
    explicit Controls2DImage(QWidget *parent = 0);
    ~Controls2DImage();

private:
    Ui::Controls2DImage *ui;
};

#endif // CONTROLS2DIMAGE_H
