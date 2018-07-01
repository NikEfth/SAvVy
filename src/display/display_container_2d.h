#ifndef DISPLAY_CONTAINER_2D_H
#define DISPLAY_CONTAINER_2D_H

#include <QWidget>

#include <memory>

#include "src/display/display_container.h"


using namespace  stir;

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container_2d : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_2d(QWidget *parent = nullptr);

   void set_array(stir::Array<2, float>* );

   void set_array(const stir::Array<2, float>& );

    /** \addtogroup Members to get access to the data
     *  @{
     */
    stir::Array<2, float>::full_iterator get_beging();

    stir::Array<2, float>::const_full_iterator get_beging_const() const ;

    stir::Array<2, float>::full_iterator  get_end();

    stir::Array<2, float>::const_full_iterator  get_end_const() const ;

     float at(int col, int row) const;
    /** @}*/

signals:

public slots:

private:

    stir::Array<2, float> data;
};

#endif // DISPLAY_CONTAINER_H
