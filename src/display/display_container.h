#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <qwt_plot.h>
#include <qwt_plot_layout.h>

//#include "common_display.h"

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
    explicit Display_container(int _my_id, int _num_dim, QWidget *parent = nullptr)
        :QwtPlot(parent), dims(_num_dim), my_id(_my_id)
    {
        this->canvas()->setMinimumSize(150, 150);
    }

    /** \addtogroup Setters
     *  @{
     */
    inline void set_file_name(QString _s)
    {
        fullFileName = _s;
    }
    /** @}*/

    /** \addtogroup Getters
     *  @{
     */
    inline int get_num_dimensions() const
    {
        return dims;
    }

    inline QString get_file_name() const
    {
        return fullFileName;
    }

    inline int get_my_id() const
    {
        return my_id;
    }
    /** @}*/

    //! Update the display contents
    //! \details Try to keep this as minimal as possible
    virtual void update_scene() = 0;

    void enable_axis(bool state = true);

signals:
    //! Signal to let the application know that this window should be
    //! removed from various places.
    void aboutToClose();

protected:
    //! File name and path
    QString fullFileName;
    //! We hack this QEvent because upon close we emit aboutToClose()
    //! to let know the application that this window must be removed
    //! from various places.
    void closeEvent(QCloseEvent *event) override;

    QwtPlotLayout* layout;
private:
    //! Number of dimensions of the data
    int dims;
    //! This number is unique within a session
    int my_id;

};

#endif // DISPLAY_CONTAINER_H
