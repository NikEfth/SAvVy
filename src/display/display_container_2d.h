#ifndef DISPLAY_CONTAINER_2D_H
#define DISPLAY_CONTAINER_2D_H

#include <QWidget>

#include <memory>

#include "display_container.h"
#include "common_display.h"

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_rescaler.h>

//!
//! \brief The Display_container class
//!

class Display_container_2d : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_2d(int, int dims = 2, QWidget *parent = nullptr);

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
    //! Set the axis. As a weird convention X axis is the
    //! vertical axis and Y the horizontal.
    void set_axis(int _offset_h, int _offset_v,
                  float _h_spacing = 1, float _v_spacing = 1);
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

public slots:
    void set_color_map(int);

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
    void set_array(const QVector<QVector<double> > &_array);
    /** @}*/

    //! QVector of data.
    //! \warning QWT plots column-wise.
    QVector<double> data;

    int row_size;

    int row_num;

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

private:

    QwtPlotSpectrogram *d_spectrogram;

    QwtMatrixRasterData *p_raster;

    QwtPlotRescaler *d_rescaler;
    //! Current ColorMap
    display::ColorMap *myColorMap;

};

#endif // DISPLAY_CONTAINER_H
