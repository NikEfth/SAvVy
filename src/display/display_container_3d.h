#ifndef DISPLAY_CONTAINER_3D_H
#define DISPLAY_CONTAINER_3D_H

#include "display_container_2d.h"

//!
//! \brief The Display_container_3d class
//!
//! This class holds a series of 2D slices. The might be 3D coordinates but they
//! are stored as a series of 1D vectors of size (col \times rows). Therefore it shares
//! a lot of common functions with Display_container_2d.
//!

class Display_container_3d : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_3d(int, int dims = 3, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array from a 2D array and update() display, by reference
    void set_display(const QVector<QVector<QVector<double> > >& , int _offset_h = 0, int _offset_v = 0, int _offset_d = 0);
    void set_display(const stir::Array<3, float>& _array);
    //!
    void set_axis(int _offset_h, int _offset_v, int _offset_d,
                  float _h_spacing= 1, float _v_spacing = 1, float _d_spacing =1 );
    /** @}*/

    inline int get_num_planes()
    { return plane_num;}
public slots:

    virtual void update_scene(int i = 0);

protected:

    /** \addtogroup Setters
     *  @{
     */
    //! Set 3D data. This function will perform the transformation to 1D
    //! Fastest option
    void set_array(const QVector<QVector<QVector<double> > > &_array);

    void set_array(const stir::Array<3, float>& _array);

    /** @}*/

    //! QVector of data.
    QVector< QVector< double > > data;

    int plane_num;

    int row_size;

    int row_num;

    int offset_h;

    int offset_v;

    int offset_d;

    //! Pixel spacing, Horizontal, this will follow the convension in the header
    float h_spacing;
    //! Pixel spacing, Vertical, this will follow the convensions in the header
    float v_spacing;

    float d_spacing;

    float origin_z;

    QVector< double > min_value;

    QVector< double > max_value;

    QwtPlotSpectrogram *d_spectrogram;

    QwtMatrixRasterData *p_raster;

    QwtPlotRescaler *d_rescaler;
    //! Current ColorMap
    display::ColorMap *myColorMap;
};

#endif // DISPLAY_CONTAINER_H
