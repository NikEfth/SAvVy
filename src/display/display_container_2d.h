#ifndef DISPLAY_CONTAINER_2D_H
#define DISPLAY_CONTAINER_2D_H

#include <QWidget>

#include <memory>
#include "display_container_raster.h"
#include "common_display.h"

//!
//! \brief The definition of Display_container_2d class
//!
//! \details The data in 2D are held by a 1D array with size [cols \times rows], which
//! is encapsulated in a QVector.The outtermost vector will never have more
//! arrays other from the first. I implemented it this way, so it can be used
//! as a parent class for the Display_container_3d.

class Display_container_2d : public Display_container_raster
{
    Q_OBJECT
public:
    explicit Display_container_2d(int, int dims = 2, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array and update() display, by pointer
    void set_display(QVector<double> * , int _row_size = 0);
    //! Set the data array and update() display, by reference
    void set_display(const QVector<double>& , int _row_size = 0);
    //! Set the data array from a 2D array and update() display, by reference.
    //! This is the fastest option with QVector
    void set_display(const QVector<QVector<double> >&);

    void set_display(const stir::Array<2, float>& );
    //!
    virtual bool set_display(void*_in);
    //! Set physical sizes of the data
    void set_sizes(int _offset_h = 0, int _offset_v = 0,
                   float _h_spacing = 1.f, float _v_spacing = 1.f,
                   float _origin_x = 0.f, float  _origin_y = 0.f);
    //! Set data and physical sizes
    void set_physical_display(const QVector<QVector<double> >&,
                              int _offset_h = 0, int _offset_v = 0,
                              float _h_spacing = 1.f, float _v_spacing = 1.f,
                              float _origin_x = 0.f, float  _origin_y = 0.f);
    //! Set the axis. As a weird convention X axis is the
    //! vertical axis and Y the horizontal.
    void set_axis(int _offset_h = 0, int _offset_v = 0,
                  float _h_spacing = 1, float _v_spacing = 1);
    /** @}*/

    /** \addtogroup Getters
     *  @{
     */

    inline int get_min_index_h() const
    {return offset_h; }

    inline int get_max_index_h()
    {return offset_h + row_size;}

    inline int get_min_index_v() const
    {return offset_v;}

    inline int get_max_index_v()
    {return offset_v + row_num; }
    /** @}*/

    void clear();

public slots:
    void set_color_map(int);

    virtual void update_scene(int i = 0);

protected:

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array and initialise x_data by pointer
    //! \warning In order to find the row_num a division is performed!
    void set_array(QVector<double>* , int _row_size = 0);
    //! Set the data array and initialise x_data by reference
    //! \warning In order to find the row_num a division is performed!
    void set_array(const QVector<double>&, int _row_size = 0 );
    //! Set 2D data. This function will perform the transformation to 1D
    //! Fastest option
    void set_array(const QVector<QVector<double> > &_array);

    void set_array(QVector<QVector<double> > *_array);

    void set_array(const stir::Array<2,float> &_array);

    void set_array(stir::Array<2, float> *_array);
    /** @}*/

    //! QVector of data.
    QVector< double >* data;
};

#endif // DISPLAY_CONTAINER_H
