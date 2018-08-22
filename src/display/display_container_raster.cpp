#include "display_container_raster.h"

#include <QSettings>

#include <QMenu>
#include <QAction>

#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>

Display_container_raster::Display_container_raster(int dims, QWidget *parent)
    : Display_container(dims,parent)
{
    QSettings settings;

    d_spectrogram = new QwtPlotSpectrogram();
    d_spectrogram->setRenderThreadCount(1);
    d_spectrogram->setCachePolicy(QwtPlotRasterItem::CachePolicy::NoCache);
    d_spectrogram->attach(this);

    this->plotLayout()->setAlignCanvasToScales( true );


//    e.setAttribute(QwtScaleEngine::Inverted);
//    this->setAxisScaleEngine(QwtPlot::yLeft, &e);
        this->axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Inverted, true);

    if(settings.contains("showAxisDefault"))
    {
        bool state = settings.value("showAxisDefault").toBool();
        this->enableAxis(QwtPlot::xBottom, state);
        this->enableAxis(QwtPlot::yLeft, state);
    }

    if(settings.contains("AutoScaleImages"))
    {
        auto_scale = settings.value("AutoScaleImages").toBool();
    }

    if(settings.contains("showColorbar"))
    {
        p_colorScale = axisWidget( QwtPlot::yRight );
        p_colorScale->setColorBarEnabled( true );
        setAxisScale( QwtPlot::yRight, m_viz_min, m_viz_max);
        enableAxis( QwtPlot::yRight );
        const QFontMetrics fm( axisWidget( QwtPlot::yRight )->font() );
        QwtScaleDraw *sd = axisScaleDraw( QwtPlot::yRight );
        sd->setMinimumExtent( fm.width( "1000.0000" ) );
    }

    p_raster = new QwtMatrixRasterData();


}

void Display_container_raster::set_axis(int _offset_h, int _offset_v, int _offset_d,
                                        float _h_origin, float _v_origin, float _d_origin,
                                        float _h_spacing, float _v_spacing, float _d_spacing)
{

    offset_h = _offset_h;
    offset_v = _offset_v;
    offset_d = _offset_d;

    h_spacing = _h_spacing;
    v_spacing = _v_spacing;
    d_spacing = _d_spacing;

    p_raster->setInterval(Qt::XAxis, QwtInterval(static_cast<double>(offset_h) * h_spacing + _h_origin,
                                                 static_cast<double>(offset_h + row_size) * h_spacing + _h_origin,
                                                 QwtInterval::IncludeBorders));

    p_raster->setInterval(Qt::YAxis, QwtInterval(static_cast<double>(offset_v) * v_spacing + _v_origin,
                                                 static_cast<double>(offset_v + row_num) * v_spacing + _v_origin,
                                                 QwtInterval::IncludeBorders));
    this->canvas()->resize(row_size, row_num);
}

void Display_container_raster::set_size(int _offset_h, int _offset_v, int _offset_d)
{

    offset_h = _offset_h;
    offset_v = _offset_v;
    offset_d = _offset_d;

    p_raster->setInterval(Qt::XAxis, QwtInterval(static_cast<double>(offset_v),
                                                 static_cast<double>((offset_v + row_num) ),
                                                 QwtInterval::IncludeBorders));

    p_raster->setInterval(Qt::YAxis, QwtInterval(static_cast<double>(offset_h),
                                                 static_cast<double>(offset_h + row_size),
                                                 QwtInterval::IncludeBorders));
    this->canvas()->resize(row_size*2, row_num*2);
}

Display_container_raster::~Display_container_raster()
{
    delete d_spectrogram;
}

size_t Display_container_raster::get_x_axis_size() const
{
    return row_size;
}
