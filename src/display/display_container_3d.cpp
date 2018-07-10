#include "display_container_3d.h"

#include <QSettings>

Display_container_3d::Display_container_3d(int _my_id, int dims, QWidget *parent) :
    Display_container(_my_id, dims, parent)
{
    QSettings settings;

    d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setRenderThreadCount(0);
    d_spectrogram->setCachePolicy(QwtPlotRasterItem::CachePolicy::NoCache);

    if(settings.contains("defaultColorMap"))
        myColorMap = new display::ColorMap(settings.value("defaultColorMap").toInt());
    else
        myColorMap = new display::ColorMap();

    d_spectrogram->setColorMap(myColorMap);

    //    d_rescaler = new QwtPlotRescaler(this->canvas(),QwtPlot::yLeft, QwtPlotRescaler::Fitting);
    //    d_rescaler->setExpandingDirection(QwtPlot::xBottom, QwtPlotRescaler::ExpandBoth);
    this->setCanvasBackground(QBrush(myColorMap->get_background()));

    if(settings.contains("showAxisDefault"))
    {
        bool state = settings.value("showAxisDefault").toBool();
        this->enableAxis(QwtPlot::xBottom, state);
        this->enableAxis(QwtPlot::yLeft, state);
    }

    p_raster = new QwtMatrixRasterData();

    d_spectrogram->setData( p_raster);
    d_spectrogram->attach(this);
}

void Display_container_3d::set_axis(int _offset_h, int _offset_v, int _offset_d,
                                    float _h_spacing, float _v_spacing, float _d_spacing)
{

    offset_h = _offset_h;
    offset_v = _offset_v;
    offset_d = _offset_d;

    h_spacing = _h_spacing;
    v_spacing = _v_spacing;
    d_spacing = _d_spacing;

    p_raster->setInterval( Qt::XAxis,
                           QwtInterval( static_cast<double>(offset_v)*v_spacing,
                                        static_cast<double>( (offset_v + row_num) )*v_spacing,
                                        QwtInterval::IncludeBorders ) );
    p_raster->setInterval( Qt::YAxis, QwtInterval(static_cast<double>(offset_h)*h_spacing,
                                                  static_cast<double>(offset_h + row_size)*h_spacing,
                                                  QwtInterval::IncludeBorders ) );

    if ( static_cast<double>(offset_h + row_size)*h_spacing < static_cast<double>( (offset_v + row_num) )*v_spacing)
    {
        d_rescaler = new QwtPlotRescaler(this->canvas(),QwtPlot::xBottom, QwtPlotRescaler::Fixed);
        d_rescaler->setExpandingDirection(QwtPlot::yLeft, QwtPlotRescaler::ExpandBoth);
    }
    else if  ( static_cast<double>(offset_h + row_size)*h_spacing > static_cast<double>( (offset_v + row_num) )*v_spacing)
    {
        d_rescaler = new QwtPlotRescaler(this->canvas(),QwtPlot::yLeft, QwtPlotRescaler::Fixed);
        d_rescaler->setExpandingDirection(QwtPlot::xBottom, QwtPlotRescaler::ExpandBoth);
    }
    else
    {
        d_rescaler = new QwtPlotRescaler(this->canvas(),QwtPlot::xBottom, QwtPlotRescaler::Fixed);
        d_rescaler->setExpandingDirection(QwtPlot::yLeft, QwtPlotRescaler::ExpandBoth);
    }
}

void Display_container_3d::set_display(const QVector<QVector<QVector<double> > > &_array,
                                       int _offset_h, int _offset_v, int _offset_d)
{
    set_array(_array);
    set_axis(_offset_h, _offset_v, _offset_d);
    update_scene();

    emit setup_ready();
}

void Display_container_3d::set_display(const stir::Array<3, float>& _array)
{
    set_array(_array);
    set_axis(_array.get_min_index(), _array[0].get_min_index(), _array[0][0].get_min_index());
    update_scene();
    emit setup_ready();
}

void Display_container_3d::set_array(const QVector<QVector<QVector<double> > > &_array)
{
    plane_num = _array.size();
    row_num = _array[0].size();
    row_size = _array[0][0].size();

    data.resize(plane_num);

    for(int i = 0; i < plane_num; ++i)
        data[i].resize(row_size*row_num);

    min_value.resize(plane_num);
    min_value.fill(100000);
    max_value.resize(plane_num);
    max_value.fill(0);

    int idx = 0;

    for(int i = 0; i < plane_num; ++i)
    {
        for(int j = 0; j < row_size; ++j)
            for(int k = 0; k < row_num; ++k, ++idx)
            {
                data[i][idx] = _array[i][j][k];

                if (data[i][idx] > max_value[i])
                    max_value[i] = data[i][idx];
                if(data[i][idx]< min_value[i])
                    min_value[i] = data[i][idx] ;
            }
        idx = 0;
    }
}

void Display_container_3d::set_array(const stir::Array<3, float>& _array)
{
    plane_num = _array.size();
    row_num = _array[0].size();
    row_size = _array[0][0].size();

    data.resize(plane_num);

    for(int i = 0; i < plane_num; ++i)
        data[i].resize(row_size*row_num);

    min_value.resize(plane_num);
    min_value.fill(100000);
    max_value.resize(plane_num);
    max_value.fill(0);

    int idp = 0, idx = 0;

    for(int i = _array.get_min_index(); i <= _array.get_max_index(); ++i, ++idp)
    {
        for(int j = _array[i].get_min_index(); j <= _array[i].get_max_index(); ++j)
            for(int k = _array[i][j].get_min_index(); k <= _array[i][j].get_max_index(); ++k, ++idx)
            {
                data[idp][idx] = _array[i][j][k];

                if (data[idp][idx] > max_value[idp])
                    max_value[idp] = data[idp][idx];
                if(data[idp][idx]< min_value[idp])
                    min_value[idp] = data[idp][idx] ;
            }
        idx = 0;
    }
}

void Display_container_3d::update_scene(int i)
{
    if (i >= plane_num)
        return;
    QVector<double> s = data[i];
    p_raster->setValueMatrix(s, row_size);

    p_raster->setInterval( Qt::ZAxis, QwtInterval(min_value[i],  max_value[i]));
    d_spectrogram->setData(p_raster);
    replot();
}
