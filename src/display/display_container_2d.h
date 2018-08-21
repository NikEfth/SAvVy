#ifndef DISPLAY_CONTAINER_2D_H
#define DISPLAY_CONTAINER_2D_H

#include <QWidget>

#include <memory>
#include "display_container_raster.h"
#include "common_display.h"

//!
//! \brief The definition of Display_container_2d class
//!
//! \details The data in 2D are held by a 1D array with size [cols /times rows], which
//! is encapsulated in a QVector.The outtermost vector will never have more
//! arrays other from the first. I implemented it this way, so it can be used
//! as a parent class for the Display_container_3d.

class Display_container_2d : public Display_container_raster
{
    Q_OBJECT
public:
    explicit Display_container_2d(int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const QVector<double>&, unsigned int row_size, int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const QVector< QVector<double> >&, int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const QVector<QVector< QVector<double> > >&, int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const stir::Array<1, float>&, unsigned int row_size, int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const  stir::Array<2, float>&, int dims = 2, QWidget *parent = nullptr);

    explicit Display_container_2d(const  stir::Array<3, float>&, int dims = 2, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>&, unsigned int row_size) override;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const QVector< QVector<double> >&) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<QVector< QVector<double> > >&) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const stir::Array<1, float>&, unsigned int _row_size) override;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const  stir::Array<2, float>&) override;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const  stir::Array<3, float>&) override;
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

//    void clear();

    virtual ~Display_container_2d() override;

    virtual void get_min_max(double& min, double& max) const override;

    virtual void set_min_max(const double min, const double max) override;

public slots:

    virtual void update_scene(int i = 0);

protected:

    void initialise();

};

#endif // DISPLAY_CONTAINER_H
