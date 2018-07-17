#ifndef HISTOGRAM_MANAGER_H
#define HISTOGRAM_MANAGER_H

#include <QWidget>
#include <memory>
#include <QFile>

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

    /** \addtogroup Setters
     *  @{
     */
    virtual void set_display(void*_in);

    virtual void set_display(std::shared_ptr<QVector<double> > _in);

    virtual void set_display(const QString& _inFIle);

    virtual void set_no_controls(bool);
    /** @}*/

    /** \addtogroup Getters
     *  @{
     */

    Display_container_bar * get_display();
    /** @}*/

    void setNumBin(const int& _n);

    void setCutOff(const float& _n);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

public slots:

    void refresh_display();

signals:



private:
    Ui::Histogram_manager *ui;

    Display_container_bar * _histogram;

    QString _input_file;

    std::shared_ptr<QVector<double> > _local_copy_ptr = nullptr;
};

#endif // HISTOGRAM_MANAGER_H
