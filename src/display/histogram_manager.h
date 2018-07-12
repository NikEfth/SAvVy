#ifndef HISTOGRAM_MANAGER_H
#define HISTOGRAM_MANAGER_H

#include <QWidget>
#include <memory>
#include "display_container_bar.h"

#include "common_display.h"

namespace Ui {
class Histogram_manager;
}

class Histogram_manager : public DisplayInterface
{
    Q_OBJECT

public:
    explicit Histogram_manager(int _my_id, int _num_dim, QWidget *parent = 0);
    ~Histogram_manager();

    virtual void set_display(void*_in);

    virtual void set_display(std::shared_ptr<QVector<double> >_in);

    virtual void set_display(const QString &_fileName);

    void setNumBin(const int& _n);

    void setCutOff(const float& _n);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::Histogram_manager *ui;

    Display_container_bar * _histogram;

};

#endif // HISTOGRAM_MANAGER_H
