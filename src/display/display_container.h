#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <qwt_plot.h>

#include "common_display.h"

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
class Display_container : public QwtPlot, public DisplayInterface
{
    Q_OBJECT
public:
    explicit Display_container(int _my_id, int _num_dim, QWidget *parent = nullptr)
        :QwtPlot(parent), DisplayInterface(_my_id, _num_dim)
    {
        this->canvas()->setMinimumSize(150, 150);
    }

    /** \addtogroup Setters
     *  @{
     */
    inline void set_file_name(QString _s)
    {
        fullFileName = _s;
        this->setWindowTitle(_s);
    }
    inline void rename(const QString& _s)
    {
        set_file_name(_s);
    }
    /** @}*/

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

};

#endif // DISPLAY_CONTAINER_H
