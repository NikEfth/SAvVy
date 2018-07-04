#include "display_container_2d.h"

#include <QSettings>

Display_container_2d::Display_container_2d(int _my_id,  int dims, QWidget *parent) :
    Display_container(_my_id, dims,parent)
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

double Display_container_2d::at(int row, int col)
{
    return data[(col + abs(offset_h)) + (row + abs(offset_v) )* row_size];
}

void Display_container_2d::set_array(QVector<double>* _array,
                                     int _row_size)
{
    data = * _array;
    row_size = _row_size;
    row_num = data.size() / row_size;
    max_value = *std::max_element(data.begin(), data.end());
    min_value = *std::min_element(data.begin(), data.end());
}

void Display_container_2d::set_array(const QVector<double> &_array,
                                     int _row_size)
{
    data = _array;

    row_size = _row_size;
    row_num = data.size() / row_size;

    max_value = *std::max_element(data.begin(), data.end());
    min_value = *std::min_element(data.begin(), data.end());
}

void Display_container_2d::set_array(const QVector<QVector<double> > &_array)
{
    row_num = _array.size();
    row_size = _array[0].size();

    data.resize(row_size*row_num);

    min_value = 100000;
    max_value = 0;

    int idx = 0;
    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            data[idx] = _array[i][j];

            if (data[idx] > max_value)
                max_value = data[idx] ;
            if(data[idx] < min_value)
                min_value = data[idx] ;
        }
}

void Display_container_2d::set_display(QVector<double>* _array,
                                       int _row_size, int _offset_h, int _offset_v)
{
    set_array(_array, _row_size);

    set_axis(_offset_h, _offset_v);
    update_scene();
}

void Display_container_2d::set_display(const QVector<double> &_array,
                                       int _row_size,
                                       int _offset_h, int _offset_v)
{
    set_array(_array, _row_size);
    set_axis(_offset_h, _offset_v);

    update_scene();
}


void Display_container_2d::set_display(const QVector<QVector<double> > &_array,
                                       int _offset_h, int _offset_v)
{
    row_num = _array.size();
    row_size = _array[0].size();

    data.resize(row_size*row_num);

    min_value = 100000;
    max_value = 0;

    int idx = 0;
    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            data[idx] = _array[i][j];

            if (data[idx] > max_value)
                max_value = data[idx] ;
            if(data[idx] < min_value)
                min_value = data[idx] ;
        }

    set_axis(_offset_h, _offset_v);
    update_scene();
}

void Display_container_2d::set_sizes(
        int _offset_h, int _offset_v,
        float _h_spacing, float _v_spacing,
        float _origin_x, float  _origin_y)
{
    set_axis(_offset_h, _offset_v,
             _h_spacing, _v_spacing);

    origin_x = _origin_x;
    origin_y = _origin_y;

}

void Display_container_2d::set_physical_display(const QVector<QVector<double> > &_array,
                                                int _offset_h, int _offset_v,
                                                float _h_spacing, float _v_spacing,
                                                float _origin_x, float  _origin_y)
{
    set_array(_array);
    set_sizes(_offset_h, _offset_v, _h_spacing, _v_spacing, _origin_x,  _origin_y);
    update_scene();
}

void Display_container_2d::set_axis(int _offset_h, int _offset_v,
                                    float _h_spacing, float _v_spacing)
{

    offset_h = _offset_h;
    offset_v = _offset_v;

    h_spacing = _h_spacing;
    v_spacing = _v_spacing;

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


void Display_container_2d::update_scene()
{
    p_raster->setValueMatrix(data, row_size);
    p_raster->setInterval( Qt::ZAxis, QwtInterval(min_value,  max_value));
    d_spectrogram->setData(p_raster);
    replot();
//    d_rescaler->rescale();
}

void Display_container_2d::clear()
{
    data.clear();

    row_size = -1;
    row_num = -1;
}

void Display_container_2d::set_color_map(int index)
{
    myColorMap->setColormap(index);
    d_spectrogram->setColorMap(myColorMap);

    //    title.setColor(myColorMap->get_peak_color());
    //    lOCD.setColor(myColorMap->get_peak_color());
    //    rOCD.setColor(myColorMap->get_peak_color());
    //    tOCD.setColor(myColorMap->get_peak_color());
    //    bOCD.setColor(myColorMap->get_peak_color());
    //    setCanvasBackground(QBrush(myColorMap->get_background()));

    replot();
}
