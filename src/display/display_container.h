#ifndef DISPLAY_CONTAINER_H
#define DISPLAY_CONTAINER_H

#include <QPoint>
#include <QEvent>
#include <QMouseEvent>

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>

#include "colormaps_display.h"
#include "stir/Array.h"
#include "stir/is_null_ptr.h"
#include "savvy.h"

#include <memory>

//!
//! \brief The Display_container class
//! \details The maximum dimention of the supported arrays is 4D. Thats the why the
//!
//! Few advice on how to create arrays:
//!
//! Initialising an stir::IndexRange<2>() is done with stir::Coordinate2D<int>(X, Y).
//! A good practice to follow is from (-row_size/2) up to (-row_size/2 + row_size). That
//! is how STIR does it.
//!
//! \todo There the size of the plots with/without the axis as default is inconsistent.
//! It the axis are enabled by default then the images are larger. Otherwise the
//! size falls to that of the minimum canvas. This creates another one inconsistency
//! with the size of the colormap preview widget.
class Display_container : public QwtPlot
{
    Q_OBJECT
public:
    explicit Display_container(int _num_dim, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set display a single vector only y valyes. X will be generated
    //! naively.
    //! \todo Currently is used and defined only for display_container_1d
    virtual void set_display(const QVector<double> & _y_array);
    //! Set a set of X and Y values.
    //! replace and after define how many curves or plots will be replaced on the process.
    //! That is particulary usefull for removed overlays.
    //! symbols plots symbols
    //! line plots a line connecting the symbols.
    virtual void set_display(const QVector<double> & _x_array,
                             const QVector<double> & _y_array,
                             bool replace = true, int after = 1,
                             bool symbols = false, bool line = false);
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<double>&, unsigned int row_size) = 0;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const QVector< QVector<double> >&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const QVector<QVector< QVector<double> > >&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const stir::Array<1, float>&, unsigned int row_size) = 0;
    //! Set the data array, initialise x_data and update() display, by pointer
    virtual void set_display(const  stir::Array<2, float>&) = 0;
    //! Set the data array, initialise x_data and update() display, by reference
    virtual void set_display(const  stir::Array<3, float>&) = 0;

    virtual void set_color_map(const QSharedPointer<QwtColorMap> i);

    /** @}*/

    /** \addtogroup Getters
     *  @{
     */
    virtual int get_num_data() const;

    virtual std::shared_ptr<QVector< QVector< double > > > get_data_all() const
    {
        return data;
    }

    virtual QVector< double > * get_data(int pos = 0) const
    {
        if (pos < data->size())
            return &(*data)[pos];
        else
            return nullptr;
    }

    virtual QVector< double > * get_current_data() const;

    inline unsigned long get_num_dimensions() const
    {
        return num_dim;
    }
    //! Show / hide axis
    inline void enable_axis(bool state = true)
    {
        this->enableAxis(QwtPlot::xBottom, state);
        this->enableAxis(QwtPlot::yLeft, state);
    }
    //! True if the axis are shown
    inline bool has_axis() const
    {
        return this->axisEnabled(QwtPlot::xBottom) || this->axisEnabled(QwtPlot::yLeft);
    }

    inline virtual QSize get_default_size() const = 0;

    virtual size_t get_x_axis_size() const = 0;

    /** @}*/

    virtual ~Display_container() override;

    void clearAllPlotItems();

    inline void setCOG(QPoint cog)
    {
        m_cog = cog;
        m_state = 0;
    }


    void resetCOG(int _state = -1);

    //! \todo
    virtual std::shared_ptr< QVector<double> >  get_x_values() const;
    //! \todo
    virtual std::shared_ptr< QVector<double> >  get_y_values() const;

    void linkTo(std::shared_ptr<Display_container> l)
    {
        link = l;
        connect(link.get(), &Display_container::display_updated,
                this, &Display_container::update_display);
    }

    void unLink()
    {
        disconnect(link.get(), &Display_container::display_updated,
                this, &Display_container::update_display);
        link  = nullptr;
    }

    //! The function is used to get the min and max value of raster data
    //! in order to scale (or not) propertly the colormap.
    //! \todo Find something meaningfull for the 1D case.
    virtual void get_min_max(double& min, double& max) const
    {}

    //! The function is used to set the min and max value of raster data
    //! in order to scale (or not) propertly the colormap.
    //! \todo Find something meaningfull for the 1D case.
    virtual void set_min_max(const double min, const double max)
    {}

public slots:
    //! Update the display contents
    //! \details Try to keep this as minimal as possible
    virtual void update_scene(int i = 0) = 0;

    virtual void update_display()
    {}

    virtual bool event( QEvent * );

signals:
    //! Signal to let the application know that this window should be
    //! removed from various places.
    void aboutToClose();

    void setup_ready();

    void display_updated();

protected:
    //! We hack this QEvent because upon close we emit aboutToClose()
    //! to let know the application that this window must be removed
    //! from various places.
    void closeEvent(QCloseEvent *event) override;
    //! Minimum value per data_num
    QVector< double >* min_value = nullptr;
    //! Maximum value per data_num
    QVector< double >* max_value = nullptr;
    //! QVector of data.
    std::shared_ptr<QVector< QVector< double > > > data;
    //! Number of dimentions of the display_container
    unsigned int num_dim;
    //! Number of datasets
    unsigned int data_num;
    //! Size of a single dataset.
    unsigned int row_size;

    QPoint m_cog;

    quint8 m_state;
    //! The minimum value of the colormap scale.
    double m_viz_min = 0.0;
    //! The maximum value of the colormap scale.
    double m_viz_max = 0.0;

    std::shared_ptr<Display_container> link = nullptr;
};

#endif // DISPLAY_CONTAINER_H
