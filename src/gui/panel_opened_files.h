#ifndef PANEL_OPENED_FILES_H
#define PANEL_OPENED_FILES_H

#include <QWidget>

namespace Ui {
class Panel_opened_files;
}

class Panel_opened_files : public QWidget
{
    Q_OBJECT

public:
    explicit Panel_opened_files(QWidget *parent = 0);
    ~Panel_opened_files();

private:
    Ui::Panel_opened_files *ui;
};

#endif // PANEL_OPENED_FILES_H
