#ifndef DISPLAY_CONTAINER_RASTER_H
#define DISPLAY_CONTAINER_RASTER_H

#include "display_container.h"

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_rescaler.h>

class Display_container_raster : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_raster(int dims, QWidget *parent = nullptr);

    virtual void set_color_map(int i)
    {
        if(!stir::is_null_ptr(myColorMap))
        {
            myColorMap->setColormap(i);
            d_spectrogram->setColorMap(myColorMap);
            replot();
        }
    }

    virtual ~Display_container_raster();
    virtual size_t get_x_axis_size() const;

protected:

    QwtPlotSpectrogram *d_spectrogram = nullptr;

    QwtMatrixRasterData *p_raster = nullptr;

    QwtPlotRescaler *d_rescaler = nullptr;
    //! Current ColorMap
    display::ColorMap *myColorMap = nullptr;
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
