#ifndef DISPLAY_CONTAINER_BAR_H
#define DISPLAY_CONTAINER_BAR_H

#include "display_container.h"
#include <qwt_plot_histogram.h>

#include <gsl/gsl_histogram.h>
#include <memory>

class Display_container_bar : public Display_container
{
    Q_OBJECT
public:
    explicit Display_container_bar(int, int dims = 0, QWidget *parent = nullptr);

    void setNumBin(const int& _n);
    void setNumBin_update(const int& _n);
    void setCutOff(const float& _n);
    void setCutOff_update(const float& _n);
    //! Set the data of which the histogram will be visualised
    //! It accepts:
    //! - QVector<double>
    virtual bool set_display(void* _in);
    //! Set the data to be histogrammed and update the hist_data contents.
    virtual void set_display(std::shared_ptr<QVector<double> > _in);
signals:

public slots:

    virtual void update_scene(int i = 0);

protected:

    int numBins;

    float cutOff;

    void initialiseHistogram();

private:

    void initialisePlotArea();

    gsl_histogram * hist_data;

    QwtPlotHistogram *d_histItem;

    QVector<QwtIntervalSample> series;

    QVector<QwtInterval> intervalA;

    std::shared_ptr<QVector<double> > local_copy;
};

#endif // DISPLAY_CONTAINER_BAR_H
