#include "display_container_1d.h"

#include <qwt_symbol.h>
#include <qwt_color_map.h>
#include <qwt_series_data.h>
#include <QSettings>
#include <qwt_plot_item.h>
#include <qwt_scale_engine.h>
#include "stir/Array.h"

Display_container_1d::Display_container_1d(int dims, QWidget *parent) :
    Display_container(dims, parent)
{
    initialise();
}

Display_container_1d::Display_container_1d(const QVector<double>& x_data, const QVector<double> & y_data, int dims, QWidget *parent) :
    Display_container(dims, parent)
{
    initialise();
    set_display(x_data, y_data);
}

Display_container_1d::Display_container_1d(const QVector<double>& _in, unsigned int row_size, int dims, QWidget *parent):
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

std::shared_ptr< QVector<double> >  Display_container_1d::get_x_values() const
{
    if(x_data != nullptr)
        return std::shared_ptr<QVector<double> >(new QVector<double>(*x_data));
    else
        return nullptr;
}

std::shared_ptr< QVector<double> >  Display_container_1d::get_y_values() const
{
    if(!stir::is_null_ptr(data))
        return std::shared_ptr<QVector<double> >( new QVector<double>(*data));
    else
        return nullptr;
}

void Display_container_1d::get_min_max(double& min, double& max) const
{

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

size_t Display_container_1d::get_x_axis_size() const
{
    return static_cast<unsigned long>(x_data->size());
}

void Display_container_1d::
initialise()
{
    QSettings settings;

    setCanvasBackground( Qt::white );

    this->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);

    this->canvas()->setPalette( QColor( "LemonChiffon" ) );

    low_thres = new QwtPlotMarker("low thres");
    low_thres->setLineStyle(QwtPlotMarker::VLine);
    low_thres->setLinePen(Qt::red, 2.0, Qt::DashLine);
    low_thres->setXValue(-1.0);
    high_thres = new QwtPlotMarker("high thres");
    high_thres->setLineStyle(QwtPlotMarker::VLine);
    high_thres->setLinePen(Qt::red, 2.0, Qt::DashLine);
    high_thres->setXValue(-1.0);

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(Qt::lightGray, 1.0);
    grid->attach( this );

    curve = new QwtPlotCurve();
    curve->setPen(Qt::red,2);

    if(settings.contains("showAxisDefault"))
    {
        bool state = settings.value("showAxisDefault").toBool();
        this->enableAxis(QwtPlot::xBottom, state);
        this->enableAxis(QwtPlot::yLeft, state);
    }

    min_value = new QVector<double>(1,100000);
    max_value = new QVector<double>(1,0);
    inc_x = 1.;
}

void Display_container_1d::set_low_thres_mark(double pos)
{
    low_thres->setXValue(pos);
    low_thres->attach(this);
    this->replot();
}

void Display_container_1d::set_high_thres_mark(double pos)
{
    high_thres->setXValue(pos);high_thres->attach(this);
    this->replot();
}

void Display_container_1d::set_display(const QVector<double> & _x_array,
                                       const QVector<double> & _y_array,
                                       bool replace,int after, bool symbols, bool line)
{

    if (replace)
    {
        QList<QwtPlotItem* > items = this->itemList(QwtPlotItem::Rtti_PlotCurve);
        if (after < items.size())
            for (int i = after; i < items.size(); ++i)
                items.at(i)->detach();
        curve = new QwtPlotCurve();
        curve->setPen(Qt::blue, 2);
    }

    if (after < 0)
    {
        QList<QwtPlotItem* > items = this->itemList(QwtPlotItem::Rtti_PlotCurve);
        if(items.size() > 1)
        {
            items.at(items.size() - 1)->detach();
        }
        return;
    }

    data = new QVector<double>(_y_array.size(), 0.0);
    (*max_value)[0] = 0.0;
    (*min_value)[0] = 10000000.0;
    savvy::copy_QVector<double>(_y_array, *data, (*min_value)[0], (*max_value)[0]);

    if(replace && after > 0)
    {
        //            double max = *std::max_element(_y_array.constBegin(), _y_array.constEnd());
        m_viz_max = std::max((*max_value)[0], m_viz_max);
        m_viz_min = std::min((*min_value)[0], m_viz_min);
    }
    else
    {
        m_viz_max = (*std::max_element(data->constBegin(), data->constEnd()));
        m_viz_min = (*std::min_element(data->constBegin(), data->constEnd()));
    }

    x_data = new QVector<double>(_x_array.size(), 0.0);
    double mi, ma;
    savvy::copy_QVector<double>(_x_array, *x_data, mi, ma);

    if (symbols)
    {
        QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                           QBrush( Qt::black ), QPen( Qt::black, 1 ), QSize( 7, 7 ) );
        curve->setSymbol( symbol );
    }

    if (!line)
    {
        curve->setStyle(QwtPlotCurve::NoCurve);
    }

    update_scene();
}

void Display_container_1d::set_display(const QVector<double> & _y_array)
{
    // to silence warning
    if(_y_array.size())
    {

    }

    //    data = new QVector<double>(_y_array.size(), 0.0);
    //    savvy::copy_QVector<double>(_y_array, *data, (*min_value)[0], (*max_value)[0]);

    //    update_scene();
}

void Display_container_1d::set_display(const QVector<double> & _array, unsigned int row_size)
{
    // to silence warning
    if(row_size)
    {

    }

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
    // to silence warning
    if(_array.size())
    {

    }

    //    data = new QVector<double>(_array.size(), 0.0);
    //    savvy::serialize_QVector<double>(_array, *data, *min_value, *max_value);
    //    calculate_x_axis();
    //    update_scene();
}

void Display_container_1d::set_display(const stir::Array<1, float>& _array, unsigned int _row_size)
{
    // to silence warning
    if(_row_size)
    {

    }

    data = new QVector<double>(static_cast<int>(_array.size()), 0.0);
    savvy::Array1D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    data_offset = _array.get_min_index();
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const  stir::Array<2, float>& _array)
{
    data = new QVector<double>(static_cast<int>(_array.size() * _array[0].size()), 0.0);
    savvy::Array2D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_display(const  stir::Array<3, float>& _array)
{
    data = new QVector<double>(static_cast<int>(_array.size() * _array[0].size() * _array[0][0].size()), 0.0);
    savvy::Array3D_QVector1D(_array, *data, (*min_value)[0], (*max_value)[0]);
    calculate_x_axis();
    update_scene();
}

void Display_container_1d::set_sizes(
        double _min_x, double  _max_x)
{
    min_x = _min_x;
    max_x = _max_x;
}


void Display_container_1d::update_scene(int i)
{
    // to silence warning
    if(i)
    {

    }

    this->setAxisScale(QwtPlot::yLeft,
                       m_viz_min,
                       m_viz_max);

    curve->setSamples(*x_data, *data);
    curve->attach(this);
    this->replot();
}


Display_container_1d::~Display_container_1d()
{
    if (x_data != nullptr)
        delete x_data;
    if(data != nullptr)
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
