#include "display_container_1d.h"

#include <qwt_symbol.h>
#include <qwt_color_map.h>

#include <QSettings>

#include "stir/Array.h"

Display_container_1d::Display_container_1d(int _my_id, int dims, QWidget *parent) :
    Display_container(_my_id, dims, parent)
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

void Display_container_1d::set_array(QVector<double>* _array)
{
    *data = * _array;
}

void Display_container_1d::set_array(const QVector<double> &_array)
{
    data = new QVector<double>(_array.size());
    *data = _array;
}

void Display_container_1d::set_array( stir::Array<1, float>* _in)
{
    data = new QVector<double>(_in->size());

    int idx = 0;
    for (int i = _in->get_min_index(); i <= _in->get_max_index(); ++i, ++idx)
        (*data)[idx] = static_cast<double>( (*_in)[i]);

     data_offset = _in->get_min_index();
}

void Display_container_1d::set_display(QVector<double>* _array)
{
    set_array(_array);

    x_data.resize(data->size());
    QVector<double>::iterator it = x_data.begin();
    double acc = 0;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
}

void Display_container_1d::set_display(const QVector<double> &_array)
{
    set_array(_array);

    x_data.resize(data->size());
    QVector<double>::iterator it = x_data.begin();
    double acc = 0;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
}

bool Display_container_1d::set_display(void* _in)
{
    stir::Array<1, float>* tmp =
            static_cast<stir::Array<1, float>* >(_in);

    if(stir::is_null_ptr(_in))
        return false;

    set_array(tmp);

    x_data.resize(data->size());
    QVector<double>::iterator it = x_data.begin();
    double acc = tmp->get_min_index();
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();

    return true;
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
    set_array(_array);

    x_data.resize(data->size());
    QVector<double>::iterator it = x_data.begin();
    double acc = min_x + inc_x/2.0;
    *it = acc; ++it;

    for (; it != x_data.end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }

    update_scene();
    emit setup_ready();
}

void Display_container_1d::update_scene(int i)
{
    this->setAxisScale(QwtPlot::yLeft,
                       *std::min_element(data->constBegin(), data->constEnd()),
                       *std::max_element(data->constBegin(), data->constEnd()));

    curve->setSamples(x_data, *data);
    curve->attach(this);
    this->replot();
}

void Display_container_1d::clear()
{
    data->clear();
    x_data.clear();
    data_offset = 0;
}
