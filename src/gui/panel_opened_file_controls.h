#ifndef PANEL_OPENED_FILE_CONTROLS_H
#define PANEL_OPENED_FILE_CONTROLS_H

#include <QWidget>

#include "controls_display_1d.h"
#include "controls_display_2d.h"

namespace Ui {
class Panel_opened_file_controls;
}

class Panel_opened_file_controls : public QWidget
{
    Q_OBJECT

public:
    explicit Panel_opened_file_controls(QWidget *parent = 0);
    ~Panel_opened_file_controls();

    void show_panel(int dim=0);

signals:
    void colormap_changed(int);

    void show_axis(bool);

    void show_cursor(bool);

private:
    Ui::Panel_opened_file_controls *ui;

    Controls_display_1d* controls_1d;

    Controls_display_2d* controls_2d;
};

#endif // PANEL_OPENED_FILE_CONTROLS_H
