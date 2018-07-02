#include "display_container_2d.h"


Display_container_2d::Display_container_2d(int _my_id, QWidget *parent, int dims) :
    Display_container(_my_id, parent, dims)
{
    d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setRenderThreadCount(0);
    d_spectrogram->setCachePolicy(QwtPlotRasterItem::CachePolicy::NoCache);

    //    this->enableAxis(QwtPlot::xBottom, false);
    //    this->enableAxis(QwtPlot::yLeft, false);

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

void Display_container_2d::set_display(QVector<double>* _array,
                                       int _row_size, int _offset_h, int _offset_v)
{
    set_array(_array, _row_size);

    offset_h = _offset_h;
    offset_v = _offset_v;

    p_raster->setInterval( Qt::YAxis,
                           QwtInterval( static_cast<double>(offset_v),
                                        static_cast<double>( offset_v + row_num),
                                        QwtInterval::ExcludeBorders ) );
    p_raster->setInterval( Qt::XAxis, QwtInterval(static_cast<double>(offset_h),
                                                  static_cast<double>(offset_h + row_size),
                                                  QwtInterval::ExcludeBorders ) );

    update_scene();
}

void Display_container_2d::set_display(const QVector<double> &_array,
                                       int _row_size,
                                       int _offset_h, int _offset_v)
{
    set_array(_array, _row_size);

    offset_h = _offset_h;
    offset_v = _offset_v;

    p_raster->setInterval( Qt::YAxis,
                           QwtInterval( static_cast<double>(offset_v),
                                        static_cast<double>( offset_v + row_num),
                                        QwtInterval::ExcludeBorders ) );
    p_raster->setInterval( Qt::XAxis, QwtInterval(static_cast<double>(offset_h),
                                                  static_cast<double>(offset_h + row_size),
                                                  QwtInterval::ExcludeBorders ) );

    update_scene();
}


void Display_container_2d::set_display(const QVector<QVector<double> > &_array,
                                       int _offset_h, int _offset_v)
{
    row_num = _array.size();
    row_size = _array[0].size();

    offset_h = _offset_h;
    offset_v = _offset_v;

    data.resize(row_size*row_size);

    min_value = 100000;
    max_value = 0;

    int idx = 0;
    for(int i = 0; i < row_size; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            data[idx] = _array[i][j];

            if (data[idx] > max_value)
                max_value = data[idx] ;
            if(data[idx] < min_value)
                min_value = data[idx] ;
        }

    p_raster->setInterval( Qt::YAxis,
                           QwtInterval( static_cast<double>(offset_v),
                                        static_cast<double>( offset_v + row_num),
                                        QwtInterval::ExcludeBorders ) );
    p_raster->setInterval( Qt::XAxis, QwtInterval(static_cast<double>(offset_h),
                                                  static_cast<double>(offset_h + row_size),
                                                  QwtInterval::ExcludeBorders ) );

    update_scene();
}

void Display_container_2d::set_sizes(
        int _offset_h, int _offset_v,
        float _h_spacing, float _v_spacing,
        float _origin_x, float  _origin_y)
{

    offset_h = _offset_h;
    offset_v = _offset_v;

    h_spacing = _h_spacing;
    v_spacing = _v_spacing;

    origin_x = _origin_x;
    origin_y = _origin_y;

    p_raster->setInterval( Qt::YAxis,
                           QwtInterval( static_cast<double>(offset_v*v_spacing),
                                        static_cast<double>( (offset_v + row_num) * v_spacing),
                                        QwtInterval::ExcludeBorders ) );
    p_raster->setInterval( Qt::XAxis, QwtInterval(static_cast<double>(offset_h*h_spacing),
                                                  static_cast<double>( (offset_h + row_size) * h_spacing),
                                                  QwtInterval::ExcludeBorders ) );

    //    setAxisScale(QwtPlot::xBottom, min_x, max_x, h_spacing);
    //    setAxisScale(QwtPlot::yLeft, offset_v * v_spacing,
    //                 (offset_v + row_size) * v_spacing, v_spacing);
}

void Display_container_2d::set_physical_display(const QVector<QVector<double> > &_array,
                                                int _offset_h, int _offset_v,
                                                float _h_spacing, float _v_spacing,
                                                float _origin_x, float  _origin_y)
{
    set_display(_array);
    set_sizes(_offset_h, _offset_v, _h_spacing, _v_spacing, _origin_x,  _origin_y);
    update_scene();
}

void Display_container_2d::update_scene()
{
    p_raster->setValueMatrix(data, row_size);
    p_raster->setInterval( Qt::ZAxis, QwtInterval(min_value,  max_value));
    d_spectrogram->setData(p_raster);

    replot();
}

void Display_container_2d::clear()
{
    data.clear();

    row_size = -1;
    row_num = -1;
}
