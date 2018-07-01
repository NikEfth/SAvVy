#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <QWidget>

#include <qwt_plot.h>

#include "common_display.h"

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container : public QwtPlot
{
    Q_OBJECT
public:
    explicit Display_container(int _my_id, QWidget *parent = nullptr, int _num_dim = 1)
        :QwtPlot(parent), dims(_num_dim), my_id(_my_id)
    { }

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array and initialise x_data by pointer
    virtual void set_array(QVector<double>* , int i = 0) = 0;
    //! Set the data array and initialise x_data by reference
    virtual void set_array(const QVector<double>&, int i = 0 ) = 0;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(QVector<double> * , int i = 0 ) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>& , int i = 0 ) = 0;
    /** @}*/

    inline int get_num_dimensions() const
    {
        return dims;
    }

    inline QString get_file_name() const
    {
        return fullFileName;
    }

    inline void set_file_name(QString _s)
    {
        fullFileName = _s;
    }

    //! Update the display contents
    //! \details Try to keep this as minimal as possible
    virtual void update_scene() = 0;

    inline int get_my_id() const
    {
        return my_id;
    }

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

private:
    //! Number of dimensions of the data
    int dims;

    int my_id;

};

#endif // DISPLAY_CONTAINER_H
