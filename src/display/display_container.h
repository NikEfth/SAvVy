#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

#include "common_display.h"
#include "stir/Array.h"
#include "stir/is_null_ptr.h"
#include "savvy.h"

using namespace display;
//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//! Few advice on how to create arrays:
//!
//! Initialising an stir::IndexRange<2>() is done with stir::Coordinate2D<int>(X, Y).
//! A good practice to follow is from (-row_size/2) up to (-row_size/2 + row_size). That
//! is how STIR does it.
//!
//! \todo There the size of the plots with/without the axis as default is inconsistent.
//! It the axis are enabled by default then the images are larger. Otherwise the
//! size falls to that of the minimum canvas. This creates another one inconsistency
//! with the size of the colormap preview widget.
class Display_container : public QwtPlot
{
    Q_OBJECT
public:
	explicit Display_container(int _num_dim, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //!
    virtual void set_display(const QVector<double> & _y_array);
    virtual void set_display(const QVector<double> & _x_array,
        const QVector<double> & _y_array, bool symbols = false);
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>&, int row_size) = 0;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const QVector< QVector<double> >&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<QVector< QVector<double> > >&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const stir::Array<1, float>&, int row_size) = 0;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const  stir::Array<2, float>&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const  stir::Array<3, float>&) = 0;

    virtual void set_display(void*) = 0;

	virtual void set_color_map(int i);

    /** @}*/

	virtual int get_num_data() const;

    inline int get_num_dimensions() const
    {
        return num_dim;
    }

    virtual ~Display_container();

public slots:
    //! Update the display contents
    //! \details Try to keep this as minimal as possible
    virtual void update_scene(int i = 0) = 0;

    void enable_axis(bool state = true);

signals:
    //! Signal to let the application know that this window should be
    //! removed from various places.
    void aboutToClose();

    void setup_ready();

protected:
    //! We hack this QEvent because upon close we emit aboutToClose()
    //! to let know the application that this window must be removed
    //! from various places.
    void closeEvent(QCloseEvent *event) override;

    QVector< double >* min_value = NULL;

    QVector< double >* max_value = NULL;

    int num_dim;

    int data_num;

    int row_size;

};

#endif // DISPLAY_CONTAINER_H
