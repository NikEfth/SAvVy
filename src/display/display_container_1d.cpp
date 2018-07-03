#include "display_container_1d.h"

#include <qwt_symbol.h>
#include <qwt_color_map.h>

#include <QSettings>

Display_container_1d::Display_container_1d(int _my_id, QWidget *parent) :
    Display_container(_my_id, parent)
{
    QSettings settings;

    setCanvasBackground( Qt::white );

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(Qt::lightGray, 1.0);
    grid->attach( this );

    curve = new QwtPlotCurve();
    curve->setPen(Qt::red,2);

    if(settings.contains("showAxisDefault"))
    {
//        bool state = settings.value("showAxisDefault").toBool();
//        this->enableAxis(QwtPlot::xBottom, state);
//        this->enableAxis(QwtPlot::yLeft, state);
    }

    inc_x = 1.;
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

    QVector<double>::iterator it = x_data.begin();
    double acc = min_x;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }
}

void Display_container_1d::set_array(const QVector<double> &_array,
                                     int offset)
{
    data = _array;
    data_offset = offset;
}

void Display_container_1d::set_display(QVector<double>* _array,
                                       int offset)
{
    set_array(_array, offset);

    x_data.resize(data.size());
    QVector<double>::iterator it = x_data.begin();
    double acc = offset;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
}

void Display_container_1d::set_display(const QVector<double> &_array,
                                       int offset)
{
    set_array(_array, offset);

    x_data.resize(data.size());
    QVector<double>::iterator it = x_data.begin();
    double acc = offset;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
}

void Display_container_1d::set_sizes(
        float _min_x, float  _max_x)
{

    min_x = _min_x;
    max_x = _max_x;
}

void Display_container_1d::set_physical_display(const QVector<double > &_array,
                                                int offset,
                                                float _min_x, float  _max_x)
{
    set_sizes(_min_x, _max_x);
    inc_x =  (fabs(max_x) + fabs(min_x)) / _array.size();
    set_array(_array, offset);

    x_data.resize(data.size());
    QVector<double>::iterator it = x_data.begin();
    double acc = min_x + inc_x/2.0;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
}

void Display_container_1d::update_scene()
{
    this->setAxisScale(QwtPlot::yLeft,
                       *std::min_element(data.constBegin(), data.constEnd()),
                       *std::max_element(data.constBegin(), data.constEnd()));

    curve->setSamples(x_data, data);
    curve->attach(this);
    this->replot();
}

void Display_container_1d::clear()
{
    data.clear();
    x_data.clear();
    data_offset = 0;
}
