#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <QWidget>

#include <memory>

#include "stir/Array.h"


using namespace  stir;

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//!

class Display_container : public QWidget
{
    Q_OBJECT
public:
    explicit Display_container(QWidget *parent = nullptr)
    {};

signals:

public slots:

private:

};

#endif // DISPLAY_CONTAINER_H
