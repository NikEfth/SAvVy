#ifndef CONTROLS_DISPLAY_2D_H
#define CONTROLS_DISPLAY_2D_H

#include <QWidget>

#include "src/display/common_display.h"

namespace Ui {
class Controls_display_2d;
}

class Controls_display_2d : public QWidget
{
    Q_OBJECT

public:
    explicit Controls_display_2d(QWidget *parent = 0);
    ~Controls_display_2d();

private:
    Ui::Controls_display_2d *ui;
};

#endif // CONTROLS_DISPLAY_2D_H