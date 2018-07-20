#include "display_container.h"

Display_container::Display_container(int _num_dim, QWidget *parent)
	:QwtPlot(parent), num_dim(_num_dim)
{
	this->canvas()->setMinimumSize(150, 150);
}

void Display_container::closeEvent(QCloseEvent *event)
{
    emit aboutToClose();
}

void Display_container::enable_axis(bool state)
{
    this->enableAxis(QwtPlot::xBottom, state);
    this->enableAxis(QwtPlot::yLeft, state);
}

Display_container::~Display_container()
{
    delete min_value;
    delete max_value;
}

int Display_container::get_num_data() const
{
	return data_num;
}

void Display_container::set_color_map(int i)
{
	//! \todo Set colormap could be usefull for 1D arrays, too.
}

void Display_container::set_display(const QVector<double> & _y_array)
{
	//!\todo It can be used for Scatter plottings
}

void Display_container::set_display(const QVector<double> & _x_array,
                                    const QVector<double> & _y_array, bool symbols)
{
    //!\todo It can be used for Scatter plottings
}
