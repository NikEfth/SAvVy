#include "display_container_3d.h"

Display_container_3d::Display_container_3d(int dims, QWidget *parent) :
    Display_container_raster(dims, parent)
{

}

Display_container_3d::Display_container_3d(const QVector<double>& _in, int row_size, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in, row_size);
}

Display_container_3d::Display_container_3d(const QVector< QVector<double> >& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in);
}

Display_container_3d::Display_container_3d(const QVector<QVector< QVector<double> > >& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in);
}

Display_container_3d::Display_container_3d(const stir::Array<1, float>& _in, int row_size, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in, row_size);
}

Display_container_3d::Display_container_3d(const  stir::Array<2, float>& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in);
}

Display_container_3d::Display_container_3d(const  stir::Array<3, float>& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    set_display(_in);
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

void Display_container_3d::set_display(const QVector<double>  &_array, int _row_size )
{
    data_num = 1;
    row_size   = _row_size;
    row_num  = _array.size()/ row_num;

    data = new QVector< QVector< double > >(1, QVector< double >( _array.size(), 0.0));
    min_value = new QVector<double>(data_num, 1000000);
    max_value = new QVector<double>(data_num, 0);

    savvy::copy_QVector(_array, (*data)[0],(*min_value)[0] ,(*max_value)[0] );

    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_3d::set_display(const QVector<QVector<double> >  &_array)
{
    //    data_num = 1;
    //    row_num  = _array.size();
    //    row_size   = _array[0].size();

    //    data = new QVector< QVector< double > >(data_num,
    //                                            QVector< double >(row_num * row_size, 0.0));
    //    min_value = new QVector<double>(data_num, 1000000);
    //    max_value = new QVector<double>(data_num, 0);

    //    savvy::serialize_QVector(_array, *data, min_value, max_value);
    //    set_axis();
    //    update_scene();

    //    emit setup_ready();
}

void Display_container_3d::set_display(const QVector<QVector<QVector<double> > > &_array)
{
    data_num = _array.size();
    row_num  = _array[0].size();
    row_size   = _array[0][0].size();

    data = new QVector< QVector< double > >(data_num,
                                            QVector< double >(row_num * row_size, 0.0));
    min_value = new QVector<double>(data_num, 1000000);
    max_value = new QVector<double>(data_num, 0);

//    savvy::serialize_QVector(_array, *data, min_value, max_value);
    set_axis();
    update_scene();

    emit setup_ready();
}

void Display_container_3d::set_display(const stir::Array<3, float>& _array)
{
    data_num = static_cast<int>(_array.size());
    row_num  = static_cast<int>(_array[0].size());
    row_size   = static_cast<int>(_array[0][0].size());

    data = new QVector< QVector< double > >(data_num,
                                            QVector< double >(row_num * row_size, 0.0));
    min_value = new QVector<double>(data_num, 1000000);
    max_value = new QVector<double>(data_num, 0);

    savvy::Array3D_QVector2Ds(_array, *data, *min_value,  *max_value);
    set_axis(_array.get_min_index(), _array[0].get_min_index(), _array[0][0].get_min_index());
    update_scene();
    emit setup_ready();
}

void Display_container_3d::set_display(void* _in)
{
    //    stir::Array<3, float>* tmp =
    //            static_cast<stir::Array<3, float>* >(_in);

    //    if(stir::is_null_ptr(tmp))
    //        return false;

    //    set_array(tmp);
    //    set_axis(tmp->get_min_index(), (*tmp)[0].get_min_index(), (*tmp)[0][0].get_min_index());
    //    update_scene();
    //    emit setup_ready();

    //    return true;
}

//void Display_container_3d::set_array(const QVector<QVector<QVector<double> > > &_array)
//{
//    plane_num = _array.size();
//    row_num = _array[0].size();
//    row_size = _array[0][0].size();

//    data = new QVector< QVector<double> >(plane_num, QVector<double>(row_num*row_size));

//    min_value = new QVector<double>(plane_num, 1000000);
//    max_value = new QVector<double>(plane_num, 0);

//    int idx = 0;

//    for(int i = 0; i < plane_num; ++i)
//    {
//        for(int j = 0; j < row_size; ++j)
//            for(int k = 0; k < row_num; ++k, ++idx)
//            {
//                float val = _array[i][j][k];
//                if (val != 0.f)
//                {
//                    (*data)[i][idx] = val;

//                    if (val > (*max_value)[i])
//                        (*max_value)[i] = val;
//                    else if(val < (*min_value)[i])
//                        (*min_value)[i] = val;
//                }
//            }
//        idx = 0;
//    }
//}

//void Display_container_3d::set_array(const stir::Array<3, float>& _array)
//{
//    plane_num = _array.size();
//    row_num = _array[0].size();
//    row_size = _array[0][0].size();

//    data = new QVector< QVector<double> >(plane_num, QVector<double>(row_num*row_size));

//    min_value = new QVector<double>(plane_num, 1000000);
//    max_value = new QVector<double>(plane_num, 0);

//    int idp = 0, idx = 0;

//    for(int i = _array.get_min_index(); i <= _array.get_max_index(); ++i, ++idp)
//    {
//        for(int j = _array[i].get_min_index(); j <= _array[i].get_max_index(); ++j)
//            for(int k = _array[i][j].get_min_index(); k <= _array[i][j].get_max_index(); ++k, ++idx)
//            {
//                float val = _array[i][j][k];
//                if (val != 0.f)
//                {
//                    (*data)[idp][idx] = static_cast<double>(val);

//                    if (val > (*max_value)[idp])
//                        (*max_value)[idp] = val;
//                    else if(val < (*min_value)[idp])
//                        (*min_value)[idp] = val;
//                }
//            }
//        idx = 0;
//    }
//}

//void Display_container_3d::set_array(const stir::Array<3, float>* _array)
//{
//    plane_num = _array->size();
//    row_num = (*_array)[0].size();
//    row_size =  (*_array)[0][0].size();

//    data = new QVector< QVector<double> >(plane_num, QVector<double>(row_num*row_size));

//    min_value = new QVector<double>(plane_num, 1000000);
//    max_value = new QVector<double>(plane_num, 0);

//    int idp = 0, idx = 0;

//    for(int i =  (*_array).get_min_index(); i <=  (*_array).get_max_index(); ++i, ++idp)
//    {
//        for(int j =  (*_array)[i].get_min_index(); j <=  (*_array)[i].get_max_index(); ++j)
//            for(int k =  (*_array)[i][j].get_min_index(); k <=  (*_array)[i][j].get_max_index(); ++k, ++idx)
//            {
//                float val =  (*_array)[i][j][k];
//                if (val != 0.f)
//                {
//                    (*data)[idp][idx] = static_cast<double>(val);

//                    if (val > (*max_value)[idp])
//                        (*max_value)[idp] = val;
//                    else if(val < (*min_value)[idp])
//                        (*min_value)[idp] = val;
//                }
//            }
//        idx = 0;
//    }
//}

void Display_container_3d::set_display(const stir::Array<1, float>& a, int row_size) 
{

}

void Display_container_3d::set_display(const  stir::Array<2, float>& a)
{

}

void Display_container_3d::update_scene(int i)
{
    if (i < data->size())
    {
        p_raster->setValueMatrix((*data)[i], row_size);

        p_raster->setInterval( Qt::ZAxis, QwtInterval((*min_value)[i],  (*max_value)[i]));
        d_spectrogram->setData(p_raster);
        replot();
    }
}

Display_container_3d::~Display_container_3d()
{
    delete data;
}
