#ifndef DISPLAY_CONTAINER_3D_H
#define DISPLAY_CONTAINER_3D_H

#include "display_container_2d.h".h"


//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container_3d : public Display_container_2d
{
    Q_OBJECT
public:
    explicit Display_container_3d(int, QWidget *parent = nullptr);


};

#endif // DISPLAY_CONTAINER_H
