#include "display_container_bar.h"
#include <QMessageBox>

Display_container_bar::Display_container_bar(int dims, QWidget *parent) :
    Display_container(dims, parent)
{
    numBins= 32;
    cutOff = 0.4f;

    initialisePlotArea();
    initialiseHistogram();
}

void Display_container_bar::setNumBin(const int& _n)
{
    numBins = _n;
    emit settings_updated();
}

int Display_container_bar::getNumBin() const
{
    return numBins;
}

void Display_container_bar::setNumBin_update(const int& _n)
{
    numBins = _n;
    // if linked
    initialiseHistogram();
    //    set_display(local_copy);
    update_scene();
}

void Display_container_bar::setCutOff(const float& _n)
{
    cutOff = _n * 0.01f;
    emit settings_updated();
}

void Display_container_bar::setCutOff_update(const float& _n)
{
    cutOff = _n * 0.01f;
    // if linked
    initialiseHistogram();
    //    set_display(local_copy);
    update_scene();
}

void Display_container_bar::initialiseHistogram()
{
    if(!stir::is_null_ptr(hist_data))
        if(hist_data->range > 0)
            gsl_histogram_free(hist_data);

    series.resize(numBins);
    intervalA.resize(numBins);
    hist_data = gsl_histogram_alloc(numBins);
}

void Display_container_bar::set_display(void* _in)
{
    QVector<double>* tmp =
            static_cast<QVector<double>* >(_in);

    if(!stir::is_null_ptr(tmp)){
        set_display(tmp);
        return ;
    }
    return ;
}

std::shared_ptr< std::vector<double> > Display_container_bar::get_bin_indices()
{
    std::shared_ptr< std::vector<double> > ret(new std::vector<double>(numBins));

    double ls = 0.0, hs = 0.0;
    for ( int j = 0; j < numBins; ++j)
    {
        gsl_histogram_get_range(hist_data, j, &ls, &hs);
        (*ret)[j] = (ls + hs) / 2.0;
    }
    return ret;
}

std::shared_ptr< std::vector<double> > Display_container_bar::get_histogram_values()
{
    std::shared_ptr< std::vector<double> > ret(new std::vector<double>(numBins));

    for ( int j = 0; j < numBins; ++j)
    {
        (*ret)[j] = gsl_histogram_get(hist_data, j);
    }
    return ret;
}

void Display_container_bar::update_scene(int i)
{
    double ls = 0.0, hs = 0.0;
    for ( int j = 0; j < numBins; ++j)
    {
        gsl_histogram_get_range(hist_data, j, &ls, &hs);
        intervalA[j].setInterval(ls, hs, QwtInterval::IncludeBorders);
        double val = gsl_histogram_get(hist_data, j);
        //        if (val > 0)
        //            maxNZBin = j;
        series[j] = QwtIntervalSample(val, intervalA[j]);
    }

    d_histItem->setData(new QwtIntervalSeriesData(series));

    float max = hs;
    gsl_histogram_get_range(hist_data, 0, &ls, &hs);
    float min = ls;
    this->setAxisScale( QwtPlot::xBottom,min,max);

    double ma = gsl_histogram_max_val(hist_data)*cutOff;
    this->setAxisScale(QwtPlot::yLeft, 0, ma);
    d_histItem->attach(this);
    d_histItem->setZ(0);
    this->replot();
}

void Display_container_bar::initialisePlotArea()
{
    hist_data = nullptr;

    d_histItem = new QwtPlotHistogram("Histogram");
    d_histItem->setRenderThreadCount(0);
    d_histItem->setZ(1);
    QBrush b(Qt::cyan);
    d_histItem->setBrush(b);
    setAutoFillBackground( true );
    this->canvas()->setPalette( QColor( "LemonChiffon" ) );
    this->setAutoReplot(true);

    this->setAxisTitle(QwtPlot::xBottom, "Value");
    this->setAxisTitle(QwtPlot::yLeft, "Number");
}


void Display_container_bar::set_display(const QVector<double>& _in, int row_size)
{
    double max = *std::max_element(_in.begin(), _in.end());
    double min = *std::min_element(_in.begin(), _in.end());
    gsl_histogram_set_ranges_uniform (hist_data, min, max);

    for(QVector<double>::ConstIterator cur = _in.begin();cur != _in.end(); ++cur)
    {
        gsl_histogram_increment (hist_data, *cur);
    }

    update_scene();
}

void Display_container_bar::set_display(QFile &_inFile)
{
    if (!_inFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Error on opening the output file.");
        msgBox.setStandardButtons(QMessageBox::Ok );
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
        return;
    }

    QTextStream _in(&_inFile);
    // Scan to find the min and max
    double min = 0.0, max = 0.0;

    double val;
    bool ok;
    {
        QString tmp = _in.readLine();
        min = tmp.toDouble(&ok);
    }
    _in.seek(0);
    while(!_in.atEnd())
    {
        QString tmp = _in.readLine();
        val = tmp.toDouble(&ok);
        if(ok)
        {
            if (val >  max )
                max = val ;
            else if(val< min)
                min = val ;
        }
    }

    _in.seek(0);
    if (min == max)
        return;

    gsl_histogram_set_ranges_uniform (hist_data, min, max);

    while(!_in.atEnd())
    {
        QString tmp = _in.readLine();
        val = tmp.toDouble(&ok);
        gsl_histogram_increment (hist_data, val);
    }
    _inFile.close();
    update_scene();
}

void Display_container_bar::append_curve(const QVector<double> & x_values,
                  const QVector< double>& y_values, const QString & name)
{
    //       Display_container_1d* tmp =  new Display_container_1d(1, this);
}

void Display_container_bar::set_display(const QVector< QVector<double> >&)
{}

void Display_container_bar::set_display(const QVector<QVector< QVector<double> > >&)
{}

void Display_container_bar::set_display(const stir::Array<1, float>&, int row_size)
{}

void Display_container_bar::set_display(const  stir::Array<2, float>&)
{}

void Display_container_bar::set_display(const  stir::Array<3, float>&)
{}