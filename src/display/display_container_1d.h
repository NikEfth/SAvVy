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

    /** \addtogroup Setters
     *  @{
     */
    //! Set display a single vector only y valyes. X will be generated
    //! naively.
    //! \todo Currently is used and defined only for display_container_1d
    virtual void set_display(const QVector<double> & _y_array) override;
    //! Set a set of X and Y values.
    //! replace and after define how many curves or plots will be replaced on the process.
    //! That is particulary usefull for removed overlays.
    //! symbols plots symbols
    //! line plots a line connecting the symbols.
    virtual void set_display(const QVector<double> & _x_array,
                             const QVector<double> & _y_array,
                             bool replace = true, int after = 1,
                             bool symbols = false, bool line = false) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>&, int row_size) override;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const QVector< QVector<double> >&) override;
    //! Set th data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<QVector< QVector<double> > >&) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const stir::Array<1, float>&, int _row_size) override;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const  stir::Array<2, float>&) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const  stir::Array<3, float>&) override;
    //!
    virtual void set_display(void*_in) override;
    //! Set the physical dimentions of the x axis
    void set_sizes(double _min_x = 0.0, double _min_y = 0.0);

    /** @}*/

    virtual size_t get_x_axis_size() const override;

    virtual ~Display_container_1d() override;

    virtual std::shared_ptr< QVector<double> >  get_x_values() const override;

    virtual std::shared_ptr< QVector<double> >  get_y_values() const override;

public slots:
    virtual void update_scene(int i = 0) override;

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

    QwtPlotCurve* curve = nullptr;

    void calculate_x_axis();

    void initialise();

};

#endif // DISPLAY_CONTAINER_H
