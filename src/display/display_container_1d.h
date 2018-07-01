#ifndef DISPLAY_CONTAINER_1D_H
#define DISPLAY_CONTAINER_1D_H

#include <QWidget>

#include <memory>

#include "src/display/display_container.h"

#include "qwt_plot.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

using namespace  stir;

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container_1d : public QwtPlot
{
    Q_OBJECT
public:
    explicit Display_container_1d(QWidget *parent = nullptr);

    void set_array(QVector<double>* , int _offset = 0);

    void set_array(const QVector<double>&, int _offset = 0 );

    void set_display(QVector<double> * , int _offset = 0 );

    void set_display(const QVector<double>& , int _offset = 0 );

    void update();

    void empty();

    /** \addtogroup Members to get access to the data
     *  @{
     */
    double* get_beging_ptr();

    double*  get_end_ptr();

    double at(int ) const;
    /** @}*/

signals:

public slots:

private:

    QVector<double> data;

    QVector<double> x_data;

    int data_offset;

    QwtPlotCurve* curve;

};

#endif // DISPLAY_CONTAINER_H
