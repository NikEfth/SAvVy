#include "display_container_2d.h"

Display_container_2d::Display_container_2d(int dims, QWidget *parent) :
    Display_container_raster(dims,parent)
{
    initialise();
}

Display_container_2d::Display_container_2d(const QVector<double>& _in, unsigned int row_size, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in, row_size);
}

Display_container_2d::Display_container_2d(const QVector< QVector<double> >& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_2d::Display_container_2d(const QVector<QVector< QVector<double> > >& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_2d::Display_container_2d(const stir::Array<1, float>& _in, unsigned int row_size, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in, row_size);
}

Display_container_2d::Display_container_2d(const  stir::Array<2, float>& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in);
}

Display_container_2d::Display_container_2d(const  stir::Array<3, float>& _in, int dims, QWidget *parent):
    Display_container_raster(dims, parent)
{
    initialise();
    set_display(_in);
}

void Display_container_2d::set_display(const QVector<double> &_array,
                                       unsigned int _row_size)
{
    row_size = _row_size;
    row_num = _array.size() / row_size;

    data.reset(new QVector< QVector< double > >(1,
                                                QVector< double >(_array.size(), 0.0)));
    savvy::copy_QVector(_array, (*data)[0],(*min_value)[0] ,(*max_value)[0] );

    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const QVector<QVector<double> > &_array)
{
    row_size = _array[0].size();
    row_num = _array.size();

    data.reset(new QVector< QVector< double > >(1,
                                                QVector< double >(row_size * row_num, 0.0)));

    savvy::serialize_QVector(_array, (*data)[0], (*min_value)[0] ,(*max_value)[0] );

    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const QVector<QVector<QVector<double> > > &_array)
{
    row_size = _array[0].size();
    row_num = _array.size();

    data.reset(new QVector< QVector< double > >(1,
                                                QVector< double >(_array.size(), 0.0)));
    savvy::serialize_QVector(_array, (*data)[0], (*min_value)[0] ,(*max_value)[0]);

    set_axis();
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const stir::Array<1, float>& _array,
                                       unsigned int _row_size)
{
    row_size = _row_size;
    row_num = static_cast<int>(_array.size()) / row_size;

    data.reset(new QVector< QVector< double > >(1,
                                                QVector< double >(row_size * row_num, 0.0)));
    savvy::Array1D_QVector1D(_array, (*data)[0], (*min_value)[0] ,(*max_value)[0]);

    set_axis(_array.get_min_index());
    update_scene();
    emit setup_ready();
}

void Display_container_2d::set_display(const stir::Array<2, float>& _array)
{
    row_num = static_cast<int>(_array.size());
    row_size = static_cast<int>(_array[0].size());

    data.reset(new QVector< QVector< double > >(1,
                                                QVector< double >(row_size * row_num, 0.0)));
    savvy::Array2D_QVector1D(_array, (*data)[0], (*min_value)[0],  (*max_value)[0]);

    set_axis(_array.get_min_index(), _array[0].get_min_index());
    update_scene();
    emit setup_ready();
}

//!\todo
void Display_container_2d::set_display(const stir::Array<3, float>& _array)
{
    // to silence warning
    if(_array.size())
    {

    }
    //    data_num =_array.size();

    //    row_size = static_cast<float>(data_num) / 2.f + 0.5f ;//_array[0][0].size() * data_num;
    //    row_num = data_num / row_size; // dat _array[0].size();

    //    QVector< QVector< double > > *tmp_data = new QVector< QVector< double > >(row_num, QVector< double >(row_size, 0.0));

    //    savvy::Array3D_QVector2D(_array, *tmp_data,  (*min_value)[0],  (*max_value)[0]);


    //    data = new QVector< double >(row_num* row_size, 0.0);

    //    savvy::Array3D_QVector1D(_array, *data,  (*min_value)[0],  (*max_value)[0]);
    //    set_axis(_array.get_min_index(), _array[0].get_min_index());
    //    update_scene();
    //    emit setup_ready();
}

void Display_container_2d::update_scene(int i)
{
    p_raster->setValueMatrix((*data)[i], row_size);

    if(auto_scale)
    {
        m_viz_min = (*min_value)[i];
        m_viz_max = (*max_value)[i];
        m_zInterval.setInterval(m_viz_min, m_viz_max);
        p_raster->setInterval( Qt::ZAxis, m_zInterval);
//        p_colorScale->setColorMap( m_zInterval, d_spectrogram->colorMap_sptr() );
    }

    d_spectrogram->setData(p_raster);

    replot();
}

//void Display_container_2d::set_color_map(int index)
//{
//    myColorMap->setColormap(index);
//    d_spectrogram->setColorMap(myColorMap);

//    //    title.setColor(myColorMap->get_peak_color());
//    //    lOCD.setColor(myColorMap->get_peak_color());
//    //    rOCD.setColor(myColorMap->get_peak_color());
//    //    tOCD.setColor(myColorMap->get_peak_color());
//    //    bOCD.setColor(myColorMap->get_peak_color());
//    //    setCanvasBackground(QBrush(myColorMap->get_background()));

//    replot();
//}

void Display_container_2d::get_min_max(double& min, double& max) const
{
    min = m_viz_min;
    max = m_viz_max;
}

void Display_container_2d::set_min_max(const double min, const double max)
{
    m_viz_min = min;
    m_viz_max = max;
}

void Display_container_2d::initialise()
{
    min_value = new QVector<double>(1,100000);
    max_value = new QVector<double>(1,0);

    data_num = 1;
    offset_d = 0;
    d_spacing = 1.f;
    origin_z = 0;
}

Display_container_2d::~Display_container_2d()
{
    data.reset();
}

