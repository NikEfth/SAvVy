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
    QwtPlotSpectrogram *d_spectrogram = NULL;

    QwtMatrixRasterData *p_raster = NULL;

    QwtPlotRescaler *d_rescaler = NULL;
    //! Current ColorMap
    display::ColorMap *myColorMap = NULL;

    int row_num;

    int offset_h;

    int offset_v;

    int offset_d;
    //! Pixel spacing, Horizontal, this will follow the convension in the header
    float h_spacing;
    //! Pixel spacing, Vertical, this will follow the convensions in the header
    float v_spacing;

    float d_spacing;

    float origin_x;

    float origin_y;

    float origin_z;

};

#endif // DISPLAY_CONTAINER_RASTER_H
