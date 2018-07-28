#ifndef DISPLAY_CONTAINER_3D_H
#define DISPLAY_CONTAINER_3D_H

#include "display_container_raster.h"

//!
//! \brief The Display_container_3d class
//!
//! This class holds a series of 2D slices. The might be 3D coordinates but they
//! are stored as a series of 1D vectors of size (col /times rows). Therefore it shares
//! a lot of common functions with Display_container_2d.
//!

class Display_container_3d : public Display_container_raster
{
    Q_OBJECT
public:
    explicit Display_container_3d(int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const QVector<double>&, int row_size, int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const QVector< QVector<double> >&, int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const QVector<QVector< QVector<double> > >&, int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const stir::Array<1, float>&, int row_size, int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const  stir::Array<2, float>&, int dims = 3, QWidget *parent = nullptr);

    explicit Display_container_3d(const  stir::Array<3, float>&, int dims = 3, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const QVector<double>& , int _row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
    void set_display(const QVector< QVector<double> >&);
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const QVector<QVector< QVector<double> > >&);
    //! Set the data array, initialise x_data and update() display, by reference
	void set_display(const stir::Array<1, float>&, int row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
	void set_display(const  stir::Array<2, float>&);
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const  stir::Array<3, float>&);
    //!
    virtual void set_display(void*_in);
    //!
    void set_axis(int _offset_h = 0, int _offset_v = 0, int _offset_d = 0,
                  float _h_spacing= 1, float _v_spacing = 1, float _d_spacing =1 );
    /** @}*/

    virtual ~Display_container_3d();
public slots:

    virtual void update_scene(int i = 0);

protected:

    //! QVector of data.
    QVector< QVector< double > > *data;

};

#endif // DISPLAY_CONTAINER_H
