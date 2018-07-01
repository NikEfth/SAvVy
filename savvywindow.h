#ifndef SAVVYWINDOW_H
#define SAVVYWINDOW_H

#include <QMainWindow>

namespace Ui {
class SavvyWindow;
}

class SavvyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SavvyWindow(QWidget *parent = 0);
    ~SavvyWindow();

private:
    Ui::SavvyWindow *ui;
};

#endif // SAVVYWINDOW_H
