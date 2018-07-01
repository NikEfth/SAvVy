#ifndef PANEL_OPENED_FILE_CONTROLS_H
#define PANEL_OPENED_FILE_CONTROLS_H

#include <QWidget>

#include "controls3dimage.h"

namespace Ui {
class Panel_opened_file_controls;
}

class Panel_opened_file_controls : public QWidget
{
    Q_OBJECT

public:
    explicit Panel_opened_file_controls(QWidget *parent = 0);
    ~Panel_opened_file_controls();

    void show_panel(int dim);

private:
    Ui::Panel_opened_file_controls *ui;

    Controls3DImage* ctrl_3D;
};

#endif // PANEL_OPENED_FILE_CONTROLS_H
