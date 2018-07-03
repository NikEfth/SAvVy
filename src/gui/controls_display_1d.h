#ifndef CONTROLS_DISPLAY_1D_H
#define CONTROLS_DISPLAY_1D_H

#include <QWidget>

namespace Ui {
class Controls_display_1d;
}

class Controls_display_1d : public QWidget
{
    Q_OBJECT

public:
    explicit Controls_display_1d(QWidget *parent = 0);
    ~Controls_display_1d();

private:
    Ui::Controls_display_1d *ui;
};

#endif // CONTROLS_DISPLAY_1D_H
