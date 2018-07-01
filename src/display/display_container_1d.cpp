#include "display_container_1d.h"

#include <qwt_symbol.h>
#include <qwt_color_map.h>

Display_container_1d::Display_container_1d(QWidget *parent) : QwtPlot(parent)
{
    setCanvasBackground( Qt::white );
    setAxisScale( QwtPlot::yLeft, 0.0, 10.0 );

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(Qt::lightGray, 1.0);
    grid->attach( this );

    curve = new QwtPlotCurve();
    curve->setPen(Qt::red,2);
}

double Display_container_1d::at(int val) const
{
    return data.at(val + abs(data_offset));
}

void Display_container_1d::set_array(QVector<double>* _array,
                                     int offset)
{
    data = * _array;
    data_offset = offset;
    x_data.resize(data.size());
    std::iota(x_data.begin(),x_data.end(), offset);
}

void Display_container_1d::set_array(const QVector<double> &_array,
                                     int offset)
{
    data = _array;
    data_offset = offset;
    x_data.resize(data.size());
    std::iota(x_data.begin(),x_data.end(), offset);
}

void Display_container_1d::set_display(QVector<double>* _array,
                                       int offset)
{
    set_array(_array, offset);
    update();
}

void Display_container_1d::set_display(const QVector<double> &_array,
                                       int offset)
{
    set_array(_array, offset);
    update();
}

double*
Display_container_1d::get_beging_ptr()
{
    return &data[0];
}

double*
Display_container_1d::get_end_ptr()
{
    return &data[data.size()];
}

void Display_container_1d::update()
{
    this->setAxisScale(QwtPlot::yLeft,
                       *std::min_element(data.constBegin(), data.constEnd()),
                       *std::max_element(data.constBegin(), data.constEnd()));

    curve->setSamples(x_data, data);
    curve->attach(this);
    this->replot();
}

void Display_container_1d::empty()
{
    data.clear();
    x_data.clear();
    data_offset = 0;
}
