#ifndef DISPLAY_CONTAINER_BAR_H
#define DISPLAY_CONTAINER_BAR_H

#include "display_container.h"
#include "display_container_1d.h"

#include <qwt_plot_histogram.h>
#include <qwt_plot_curve.h>

#include <gsl/gsl_histogram.h>
#include <memory>

#include <QFile>

class Display_container_bar : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_bar(int dims = 0, QWidget *parent = nullptr);

    /** \addtogroup Setters
     *  @{
     */
    //! Set the numBins in the histogram
    void setNumBin(const size_t &_n);
    //! As setNumBin plus update_scene
    void setNumBin_update(const size_t &_n);
    //! Set the cutOff value
    void setCutOff(const float& _n);
    //! setCutOff and update and update_scene
    void setCutOff_update(const float& _n);
    //! Set the data of which the histogram will be visualised
    //! It accepts:
    //! - QVector<double>
    virtual void set_display(void* _in);

    void set_display(QFile &_in);

    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const QVector<double>& , int _row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
    void set_display(const QVector< QVector<double> >&);
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const QVector<QVector< QVector<double> > >&);
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const stir::Array<1, float>&, int row_size);
    //! Set the data array, initialise x_data and update() display, by pointer
    void set_display(const  stir::Array<2, float>&);
    //! Set the data array, initialise x_data and update() display, by reference
    void set_display(const  stir::Array<3, float>&);

    void append_curve(const QVector<double> & x_values,
                      const QVector< double>& y_values, const QString & name);

    //    void remove_curve();

    /** @}*/

    std::shared_ptr< QVector<double> > get_bin_indices();

    std::shared_ptr< QVector<double> > get_histogram_values();

    size_t getNumBin() const ;

    void initialiseHistogram();
signals:

    void settings_updated();

public slots:

    virtual void update_scene(int i = 0);

protected:

    size_t numBins;

    float cutOff;

private:

    void initialisePlotArea();

    gsl_histogram * hist_data = nullptr;

    QwtPlotHistogram *d_histItem = nullptr;

    //    QwtCurveFitter* curve = nullptr;

    QwtPlotCurve* curve = nullptr;

    QVector<QwtIntervalSample> series;

    QVector<QwtInterval> intervalA;

    QTextStream* _input_stream = nullptr;
};

#endif // DISPLAY_CONTAINER_BAR_H
