#include "display_container_2d.h"

Display_container_2d::Display_container_2d(int _my_id,  int dims, QWidget *parent) :
    Display_container_raster(_my_id, dims,parent)
{
    min_value = new QVector<double>(1,100000);
    max_value = new QVector<double>(1,0);

    plane_num = 1;
    offset_d = 0;
    d_spacing = 1.f;
    origin_z = 0;
}

void Display_container_2d::set_array(QVector<double>* _array,
                                     int _row_size)
{
    *data = *_array;
    row_size = _row_size;
    row_num = data->size() / row_size;
    (*max_value)[0] = *std::max_element(data->begin(),data->end());
    (*min_value)[0] = *std::min_element(data->begin(), data->end());
}

void Display_container_2d::set_array(const QVector<double> &_array,
                                     int _row_size)
{
    row_size = _row_size;
    row_num = _array.size() / row_size;

    data = new QVector< double >(row_num*_row_size, 0.0);
    *data = _array;
    (*max_value)[0] = *std::max_element(data->begin(), data->end());
    (*min_value)[0] = *std::min_element(data->begin(), data->end());
}

void Display_container_2d::set_array(const QVector<QVector<double> > &_array)
{
    row_num = _array.size();
    row_size = _array[0].size();

    data = new QVector< double >(row_num*row_size, 0.0);

    int idx = 0;
    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            float val =  _array[i][j];
            if (val != 0.f)
            {
                (*data)[idx] = val;

                if (val >  (*max_value)[0] )
                    (*max_value)[0]  = val ;
                else if(val<  (*min_value)[0])
                    (*min_value)[0] = val ;
            }
        }
}

void Display_container_2d::set_array(QVector<QVector<double> > *_array)
{
    row_num = _array->size();
    row_size = _array[0].size();

    data = new QVector< double >(row_num*row_size, 0.0);

    int idx = 0;
    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            float val =  (*_array)[i][j];
            if (val != 0.f)
            {
                (*data)[idx] = val;

                if (val >  (*max_value)[0] )
                    (*max_value)[0]  = val ;
                else if(val<  (*min_value)[0])
                    (*min_value)[0] = val ;
            }
        }
}

void Display_container_2d::set_array(const stir::Array<2,float> &_array)
{
    row_num = _array.size();
    row_size = _array[0].size();

    data = new QVector< double >(row_num*row_size, 0.0);

    int idx = 0;
    for(int i = _array.get_min_index(); i <= _array.get_max_index(); ++i)
        for(int j = _array[i].get_min_index(); j <= _array[i].get_max_index(); ++j, ++idx)
        {
            float val = _array[i][j];
            if (val != 0.f)
            {
                (*data)[idx] = val ;

                if (val> (*max_value)[0] )
                    (*max_value)[0]  = val ;
                else if(val <  (*min_value)[0])
                    (*min_value)[0] = val;
            }
        }
}

void Display_container_2d::set_array(stir::Array<2,float> *_array)
{
    row_num = _array->size();
    row_size = (*_array)[0].size();

    data = new QVector< double >(row_num*row_size, 0.0);

    int idx = 0;
    for(int i = _array->get_min_index(); i <= _array->get_max_index(); ++i)
        for(int j =  (*_array)[i].get_min_index(); j <=  (*_array)[i].get_max_index(); ++j, ++idx)
        {
            float val = (*_array)[i][j];
            if (val != 0.f)
            {
                (*data)[idx] = static_cast<double>(val) ;

                if (val> (*max_value)[0] )
                    (*max_value)[0]  = val ;
                else if(val <  (*min_value)[0])
                    (*min_value)[0] = val;
            }
        }
}

void Display_container_2d::set_display(QVector<double>* _array,
                                       int _row_size)
{
    set_array(_array, _row_size);
    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const QVector<double> &_array,
                                       int _row_size)
{
    set_array(_array, _row_size);
    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const QVector<QVector<double> > &_array)
{
    set_array(_array);
    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const stir::Array<2, float>& _array)
{
    set_array(_array);
    set_axis(_array.get_min_index(), _array[0].get_min_index());
    update_scene();
    emit setup_ready();
}

bool Display_container_2d::set_display(void* _in)
{
    stir::Array<2, float>* tmp =
            reinterpret_cast<stir::Array<2, float>* >(_in);
    set_array(tmp);
    set_axis(tmp->get_min_index(), tmp[0].get_min_index());
    update_scene();
    emit setup_ready();

    return true;
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


void Display_container_2d::update_scene(int i)
{
    p_raster->setValueMatrix(*data, row_size);
    p_raster->setInterval( Qt::ZAxis, QwtInterval((*min_value)[0],  (*max_value)[0]));
    d_spectrogram->setData(p_raster);
    replot();
}

void Display_container_2d::clear()
{
    data->clear();

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
