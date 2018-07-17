#include "display_container_1d.h"

#include <qwt_symbol.h>
#include <qwt_color_map.h>

#include <QSettings>

#include "stir/Array.h"

Display_container_1d::Display_container_1d(int dims, QWidget *parent) :
    Display_container(dims, parent)
{
    initialise();
}

Display_container_1d::Display_container_1d(const QVector<double>& x_data,
                                           const QVector<double> & y_data, int dims, QWidget *parent) :
    Display_container(dims, parent)
{
    initialise();
    set_display(x_data, y_data);
}

Display_container_1d::Display_container_1d(const QVector<double>& _in, int row_size, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in, row_size);
}

Display_container_1d::Display_container_1d(const QVector< QVector<double> >& _in, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_1d::Display_container_1d(const QVector<QVector< QVector<double> > >& _in, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_1d::Display_container_1d(const stir::Array<1, float>& _in, int row_size, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in, row_size);
}

Display_container_1d::Display_container_1d(const  stir::Array<2, float>& _in, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_1d::Display_container_1d(const  stir::Array<3, float>& _in, int dims, QWidget *parent):
    Display_container(dims, parent)
{
    initialise();
    set_display(_in);
}


void Display_container_1d::initialise()
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

    min_value = new QVector<double>(1,100000);
    max_value = new QVector<double>(1,0);
    inc_x = 1.;
}

void Display_container_1d::set_display(const QVector<double> & _x_array,
                                       const QVector<double> & _y_array)
{
    data = new QVector<double>(_y_array.size(), 0.0);
    savvy::copy_QVector<double>(_y_array, *data, (*min_value)[0], (*max_value)[0]);

    x_data = new QVector<double>(_x_array.size(), 0.0);
    savvy::copy_QVector<double>(_x_array, *x_data, (*min_value)[0], (*max_value)[0]);

    update_scene();
}

void Display_container_1d::set_display(const QVector<double> & _array, int row_size)
{
    data = new QVector<double>(_array.size(), 0.0);
    savvy::copy_QVector<double>(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const QVector<QVector<double> >&  _array)
{
    data = new QVector<double>(_array.size(), 0.0);
    savvy::serialize_QVector<double>(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const QVector<QVector<QVector<double> > >&  _array)
{
    //    data = new QVector<double>(_array.size(), 0.0);
    //    savvy::serialize_QVector<double>(_array, *data, *min_value, *max_value);
    //    calculate_x_axis();
    //    update_scene();
}

void Display_container_1d::set_display(const stir::Array<1, float>& _array, int _row_size)
{
    data = new QVector<double>(_array.size(), 0.0);
    savvy::Array1D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    data_offset = _array.get_min_index();
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const  stir::Array<2, float>& _array)
{
    data = new QVector<double>(_array.size()* _array[0].size(), 0.0);
    savvy::Array2D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const  stir::Array<3, float>& _array)
{
    data = new QVector<double>(_array.size()* _array[0].size() * _array[0][0].size(), 0.0);
    savvy::Array3D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(void* _in)
{
    //    stir::Array<1, float>* tmp =
    //            static_cast<stir::Array<1, float>* >(_in);

    //    if(stir::is_null_ptr(_in))
    //        return false;

    //    set_array(tmp);

    //    x_data.resize(data->size());
    //    QVector<double>::iterator it = x_data.begin();
    //    double acc = tmp->get_min_index();
    //    *it = acc; ++it;

    //    for (; it != x_data.end(); ++it)
    //    {
    //        acc += inc_x;
    //        *it = acc;
    //    }

    //    update_scene();

    //    return true;
}

void Display_container_1d::set_sizes(
        float _min_x, float  _max_x)
{
    min_x = _min_x;
    max_x = _max_x;
}


void Display_container_1d::update_scene(int i)
{
    this->setAxisScale(QwtPlot::yLeft,
                       *std::min_element(data->constBegin(), data->constEnd()),
                       *std::max_element(data->constBegin(), data->constEnd()));

    curve->setSamples(*x_data, *data);
    curve->attach(this);
    this->replot();
}

void Display_container_1d::clear()
{
    data->clear();
    x_data->clear();
    data_offset = 0;
}

Display_container_1d::~Display_container_1d()
{
    delete x_data;
    delete data;
}

void Display_container_1d::calculate_x_axis()
{
    x_data = new QVector<double>(data->size(), 0.0);
    QVector<double>::iterator it = x_data->begin();
    double acc = data_offset;
    *it = acc; ++it;

    for (; it != x_data->end(); ++it)
    {
        acc += inc_x;
        *it = acc;
    }
}
