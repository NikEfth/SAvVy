#include "display_container_raster.h"

#include <QSettings>

Display_container_raster::Display_container_raster(int _my_id,  int dims, QWidget *parent)
    : Display_container(_my_id, dims,parent)
{
    QSettings settings;

    d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setRenderThreadCount(1);
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

    d_spectrogram->attach(this);
}
