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
    explicit Controls_display_2d(const QStringList &colorMapNames, QWidget *parent = nullptr);
    ~Controls_display_2d();

signals:
    void colormap_changed(int);

private:
    Ui::Controls_display_2d *ui;
};

#endif // CONTROLS_DISPLAY_2D_H
