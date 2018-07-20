#ifndef DISPLAY_CONTAINER_1D_H
#define DISPLAY_CONTAINER_1D_H

#include <QWidget>

#include "display_container.h"
#include "stir/Array.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

//!
//! \brief The display class for 1D graphs
//!

class Display_container_1d : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_1d(int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const QVector<double>& x_data, const QVector<double> & y_data, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const QVector<double>&, int row_size, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const QVector< QVector<double> >&, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const QVector<QVector< QVector<double> > >&, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const stir::Array<1, float>&, int row_size, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const  stir::Array<2, float>&, int dims = 1, QWidget *parent = nullptr);

    explicit Display_container_1d(const  stir::Array<3, float>&, int dims = 1, QWidget *parent = nullptr);

    //    virtual Display_container_1d* clone() const;
    /** \addtogroup Setters
     *  @{
     */
    virtual void set_display(const QVector<double> & _y_array);

    virtual void set_display(const QVector<double> & _x_array,
                     const QVector<double> & _y_array, bool symbols = false);
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>&, int row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const QVector< QVector<double> >&);
    //! Set th data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<QVector< QVector<double> > >&);
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const stir::Array<1, float>&, int _row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const  stir::Array<2, float>&);
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const  stir::Array<3, float>&);
    //!
    virtual void set_display(void*_in);
    //! Set the physical dimentions of the x axis
    void set_sizes(float _min_x = 0.f, float  _min_y = 0.f);

    /** @}*/


    virtual ~Display_container_1d();
    //! Clear the data and x_data
    void clear();

public slots:
    virtual void update_scene(int i = 0);

private:
    //! QVector of y data
    QVector<double>* data = nullptr;
    //! QVector of x data
    QVector<double>* x_data = nullptr;
    //! Offset of the x axis
    int data_offset;
    //! The minimum x value in physical dimensions
    double min_x;
    //! The maximum x value in physical dimensions
    double max_x;
    //! The incremental step along the x axis
    double inc_x;

    QwtPlotCurve* curve;

    void calculate_x_axis();

    void initialise();

};

#endif // DISPLAY_CONTAINER_H
