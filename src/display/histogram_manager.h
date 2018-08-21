#ifndef HISTOGRAM_MANAGER_H
#define HISTOGRAM_MANAGER_H

#include <QWidget>
#include <memory>
#include <QFile>

#include "display_container_bar.h"

#include "common_display.h"

using namespace display;

namespace Ui {
class Histogram_manager;
}

class Histogram_manager : public DisplayInterface
{
    Q_OBJECT

public:
    explicit Histogram_manager(int _my_id, int _num_dim, QWidget *parent = nullptr);
    ~Histogram_manager();

    /** \addtogroup Setters
     *  @{
     */
    virtual void set_display(void*_in);

    virtual void set_display(std::shared_ptr<QVector<double> > _in);

    virtual void set_display(const QString& _inFIle);

    virtual void set_no_controls(bool);

    virtual void set_curve(const QVector<double> & _x_array,
                           const QVector<double> & _y_array,
                           bool replace = true, int after = 1,
                           bool symbols = false, bool line = false) override;
    /** @}*/

    /** \addtogroup Getters
     *  @{
     */

    shared_ptr<Display_container_bar> get_display() const;
    /** @}*/

    virtual size_t get_num_points() const override;

    void setNumBin(const size_t& _n);

    void setCutOff(const float& _n);

    virtual std::shared_ptr< QVector<double> >  get_x_values() const override;

    virtual std::shared_ptr< QVector<double> >  get_y_values() const override;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

public slots:

    void refresh_display();

signals:



private:
    Ui::Histogram_manager *ui = nullptr;

    shared_ptr<Display_container_bar> _histogram = nullptr;

    QString _input_file;

    std::shared_ptr<QVector<double> > _local_copy_ptr = nullptr;
};

#endif // HISTOGRAM_MANAGER_H
