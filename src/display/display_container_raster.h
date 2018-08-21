#ifndef DISPLAY_CONTAINER_RASTER_H
#define DISPLAY_CONTAINER_RASTER_H

#include "display_container.h"

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_rescaler.h>
#include <qwt_scale_widget.h>

#include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_engine.h>

class Display_container_raster : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_raster(int dims, QWidget *parent = nullptr);

    inline void set_color_map(const QSharedPointer<QwtColorMap> cm) override
    {
        if(cm != nullptr)
        {
            d_spectrogram->setColorMap(cm);
            p_colorScale->setColorMap( m_zInterval, d_spectrogram->colorMap_sptr() );
            replot();
        }
    }

    inline QSize get_default_size() const override
    {
        return  QSize(row_num, row_size);
    }

    virtual ~Display_container_raster() override;

    virtual size_t get_x_axis_size() const override;

    //    void set_min_max(const double min,
    //                     const double max);
protected slots:

    //    virtual void popUpMenu(const QPoint &pos) override;

protected:
    //! Set the axis. As a weird convention X axis is the
    //! vertical axis and Y the horizontal.
    void set_axis(int _offset_h = 0, int _offset_v = 0, int _offset_d = 0,
                  float _h_origin = 0.f, float _v_origin = 0.f, float _d_origin = 0.f,
                  float _h_spacing= 1.f, float _v_spacing = 1.f, float _d_spacing = 1.f );

    void set_size(int _offset_h = 0, int _offset_v = 0, int _offset_d = 0);

    bool cm_set = false;
    //! Scale the colormap to the max and min of the
    //! current frame.
    bool auto_scale = true;

    QwtPlotSpectrogram *d_spectrogram = nullptr;

    QwtMatrixRasterData *p_raster = nullptr;

    QwtScaleWidget *p_colorScale = nullptr;

    QwtLinearScaleEngine e;

    QwtInterval m_zInterval;
    //! Number of rows of data. - Nomrally Y axis
    unsigned long row_num;
    //! Offset of the horizontal axis
    int offset_h;
    //! Offset of the vertical axis
    int offset_v;
    //! Offeset of the depth - z axis. Meaningly only in the Display_container_3d.
    int offset_d;
    //! Pixel spacing, Horizontal, this will follow the convension in the header
    float h_spacing;
    //! Pixel spacing, Vertical, this will follow the convensions in the header
    float v_spacing;
    //! Pixel spacing on the z axis.
    float d_spacing;
    //! Origin on the x axis.
    float origin_x;
    //! Origin on the y axis.
    float origin_y;
    //! Origin on the z axis.
    float origin_z;


};

#endif // DISPLAY_CONTAINER_RASTER_H
