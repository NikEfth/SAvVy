#ifndef DISPLAY_CONTAINER_3D_H
#define DISPLAY_CONTAINER_3D_H

#include "display_container_raster.h"

//!
//! \brief The Display_container_3d class
//!
//! This class holds a series of 2D slices. The might be 3D coordinates but they
//! are stored as a series of 1D vectors of size (col \times rows). Therefore it shares
//! a lot of common functions with Display_container_2d.
//!

class Display_container_3d : public Display_container_raster
{
    Q_OBJECT
public:
    explicit Display_container_3d(int, int dims = 3, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array from a 2D array and update() display, by reference
    void set_display(const QVector<QVector<QVector<double> > >&);
    //!
    void set_display(const stir::Array<3, float>& _array);
    //!
    virtual bool set_display(void*_in);
    //!
    void set_axis(int _offset_h = 0, int _offset_v = 0, int _offset_d = 0,
                  float _h_spacing= 1, float _v_spacing = 1, float _d_spacing =1 );
    /** @}*/

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

    void set_array(const stir::Array<3, float>* _array);

    /** @}*/

    //! QVector of data.
    QVector< QVector< double > > *data;

};

#endif // DISPLAY_CONTAINER_H
