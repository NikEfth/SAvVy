#include "display_container.h"

void Display_container::closeEvent(QCloseEvent *event)
{
    emit aboutToClose();
}

void Display_container::enable_axis(bool state)
{
    this->enableAxis(QwtPlot::xBottom, state);
    this->enableAxis(QwtPlot::yLeft, state);
}
