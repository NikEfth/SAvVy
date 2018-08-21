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

void Display_container_3d::set_display(const QVector<double>  &_array, unsigned int _row_size )
{
    data_num = 1;
    row_size = _row_size;
    row_num  = _array.size()/ row_num;

    data.reset(new QVector< QVector< double > >(1, QVector< double >( _array.size(), 0.0)));
    min_value = new QVector<double>(data_num, 1000000);
    max_value = new QVector<double>(data_num, 0);

    savvy::copy_QVector(_array, (*data)[0],(*min_value)[0] ,(*max_value)[0] );

    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_3d::set_display(const QVector<QVector<double> >  &_array)
{
    // to silence warning
    if(_array.size())
    {

    }

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
    data_num = static_cast<unsigned int>(_array.size());
    row_num  = static_cast<unsigned int>(_array[0].size());
    row_size = static_cast<unsigned int>(_array[0][0].size());

    data.reset(new QVector< QVector< double > >(data_num,
                                            QVector< double >(row_num * row_size, 0.0)));
    min_value = new QVector<double>(data_num, 1000000);
    max_value = new QVector<double>(data_num, 0);

    savvy::serialize_QVector<double>(_array, *data, *min_value, *max_value);

    set_axis();
    update_scene();

    emit setup_ready();
}

void Display_container_3d::set_display(const stir::Array<3, float>& _array)
{
    data_num = _array.size();
    row_num  = _array[0].size();
    row_size = _array[0][0].size();

    data.reset(new QVector< QVector< double > >(data_num,
                                            QVector< double >(row_num * row_size, 0.0)));
    min_value = new QVector<double>(data_num, 1000000.0);
    max_value = new QVector<double>(data_num, 0.0);

    savvy::Array3D_QVector2Ds(_array, *data, *min_value,  *max_value);
    const stir::VoxelsOnCartesianGrid<float> *t = dynamic_cast<const stir::VoxelsOnCartesianGrid<float>* >(&_array);
    if (stir::is_null_ptr(t))
    {
        set_size(_array.get_min_index(), _array[0].get_min_index(), _array[0][0].get_min_index());
    }
    else
    {
        stir::BasicCoordinate<3,int> i = t->get_min_indices();

        if (this->axisEnabled(QwtPlot::yLeft))
        {
            stir::BasicCoordinate<3,float> b = t->get_grid_spacing();
            stir::BasicCoordinate<3,float> o = t->get_origin();

            set_axis(i[3],i[2], i[1],
                    o[3], o[2], o[1],
                    b[3], b[2], b[1]);
        }
        else
            set_size(i[3],i[2], i[1]);
    }
    update_scene();

    emit setup_ready();
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

void Display_container_3d::set_display(const stir::Array<1, float>& a, unsigned int row_size)
{
    // to silence warning
    if(a.size() || row_size)
    {

    }
}

void Display_container_3d::set_display(const  stir::Array<2, float>& a)
{
    // to silence warning
    if(a.size())
    {

    }
}

void Display_container_3d::update_scene(int i)
{
    if (i < data->size())
    {
        current_slice = i;
        p_raster->setValueMatrix((*data)[current_slice], static_cast<int>(row_size));
        d_spectrogram->setData(p_raster);

        if(auto_scale)
        {
            m_viz_min = (*min_value)[current_slice];
            m_viz_max = (*max_value)[current_slice];
            m_zInterval.setInterval(m_viz_min, m_viz_max);
            p_raster->setInterval( Qt::ZAxis, m_zInterval);

            setAxisScale( QwtPlot::yRight, m_zInterval.minValue(), m_zInterval.maxValue());
            p_colorScale->setColorMap( m_zInterval, d_spectrogram->colorMap_sptr());
        }

        emit display_updated();
        replot();
    }
}

QVector< double > * Display_container_3d::get_current_data() const
{
    return get_data(current_slice);
}

void Display_container_3d::get_min_max(double& min, double& max) const
{
    min = m_viz_min;
    max = m_viz_max;
}

void Display_container_3d::set_min_max(const double min, const double max)
{
    m_viz_min = min;
    m_viz_max = max;
}

Display_container_3d::~Display_container_3d()
{
    data.reset();
}
