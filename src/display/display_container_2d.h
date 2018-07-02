#ifndef DISPLAY_CONTAINER_2D_H
#define DISPLAY_CONTAINER_2D_H

#include <QWidget>

#include <memory>

#include "src/display/display_container.h"

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container_2d : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_2d(int, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array and update() display, by pointer
    void set_display(QVector<double> * , int _row_size = 0, int _offset_h = 0, int _offset_v = 0);
    //! Set the data array and update() display, by reference
    void set_display(const QVector<double>& , int _row_size = 0 , int _offset_h = 0, int _offset_v = 0);
    //! Set the data array from a 2D array and update() display, by reference
    void set_display(const QVector<QVector<double> >& , int _offset_h = 0, int _offset_v = 0);
    //! Set physical sizes of the data
    void set_sizes(int _offset_h = 0, int _offset_v = 0,
                   float _h_spacing = 1.f, float _v_spacing = 1.f,
                   float _origin_x = 0.f, float  _origin_y = 0.f);
    //! Set data and physical sizes
    void set_physical_display(const QVector<QVector<double> >&,
                              int _offset_h = 0, int _offset_v = 0,
                              float _h_spacing = 1.f, float _v_spacing = 1.f,
                              float _origin_x = 0.f, float  _origin_y = 0.f);
    /** @}*/

    /** \addtogroup Members to get access to the data
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

    double at(int row, int col);
    /** @}*/

    virtual void update_scene();

    void clear();

protected:

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array and initialise x_data by pointer
    //! \warning In order to find the row_num a division is performed!
    virtual void set_array(QVector<double>* , int _row_size = 0);
    //! Set the data array and initialise x_data by reference
    //! \warning In order to find the row_num a division is performed!
    virtual void set_array(const QVector<double>&, int _row_size = 0 );
    /** @}*/

private:
    //! QVector of y data
    QVector<double> data;

    int row_size;

    int row_num;

    QwtPlotSpectrogram *d_spectrogram;

    QwtMatrixRasterData *p_raster;

    int offset_h;

    int offset_v;
    //! Pixel spacing, Horizontal, this will follow the convension in the header
    float h_spacing;
    //! Pixel spacing, Vertical, this will follow the convensions in the header
    float v_spacing;

    float origin_x;

    float  origin_y;

    double min_value;

    double max_value;

};

#endif // DISPLAY_CONTAINER_H
