#ifndef DISPLAY_CONTAINER_3D_H
#define DISPLAY_CONTAINER_3D_H

#include "display_container_2d.h"

#include <QSlider>

//!
//! \brief The Display_container_3d class
//!
//! This class holds a series of 2D slices. The might be 3D coordinates but they
//! are stored as a series of 1D vectors of size (col \times rows). Therefore it shares
//! a lot of common functions with Display_container_2d.
//!

class Display_container_3d : public Display_container_2d
{
    Q_OBJECT
public:
    explicit Display_container_3d(int, int dims = 3, QWidget *parent = nullptr);

private:
};

#endif // DISPLAY_CONTAINER_H
