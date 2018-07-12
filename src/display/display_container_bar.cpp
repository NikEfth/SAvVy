#include "display_container_bar.h"

Display_container_bar::Display_container_bar(int _my_id, int dims, QWidget *parent) :
    Display_container(_my_id, dims, parent)
{
    numBins= 32;
    cutOff = 0.4;

    initialisePlotArea();
    initialiseHistogram();
}

void Display_container_bar::setNumBin(const int& _n)
{
    numBins = _n;
}

void Display_container_bar::setNumBin_update(const int& _n)
{
    numBins = _n;
    initialiseHistogram();
    set_display(local_copy);
    update_scene();
}

void Display_container_bar::setCutOff(const float& _n)
{
    cutOff = _n * 0.01f;
}

void Display_container_bar::setCutOff_update(const float& _n)
{
    cutOff = _n * 0.01f;
    initialiseHistogram();
    set_display(local_copy);
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

bool Display_container_bar::set_display(void* _in)
{
    QVector<double>* tmp =
            static_cast<QVector<double>* >(_in);

    if(!stir::is_null_ptr(tmp)){
        set_display(tmp);
        return true;
}
    return false;
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

void Display_container_bar::set_display(std::shared_ptr<QVector<double> > _in)
{
    local_copy = _in;
    double max = *std::max_element(_in->begin(), _in->end());
    double min = *std::min_element(_in->begin(), _in->end());
    gsl_histogram_set_ranges_uniform (hist_data, min, max);

    for(QVector<double>::ConstIterator cur = _in->begin();cur != _in->end(); ++cur)
        gsl_histogram_increment (hist_data, *cur);

    update_scene();
}
