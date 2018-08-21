#include "display_container.h"
#include <qwt_scale_engine.h>
#include <qwt_plot_renderer.h>

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPrinter>

Display_container::Display_container(int _num_dim, QWidget *parent)
    :QwtPlot(parent), num_dim(_num_dim)
{
    this->axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating, true);
}

void Display_container::closeEvent(QCloseEvent *event)
{
    // to silence warning
    if(event)
    {

    }

    emit aboutToClose();
}

Display_container::~Display_container()
{
    delete min_value;
    delete max_value;
}

int Display_container::get_num_data() const
{
    return data_num;
}

QVector< double > *
Display_container::get_current_data() const
{
    return get_data(0);
}

void Display_container::set_color_map(const QSharedPointer<QwtColorMap> i)
{
    // to silence warning
    if(i)
    {

    }

    //! \todo Set colormap could be usefull for 1D arrays, too.
}

void Display_container::set_display(const QVector<double> & _y_array)
{
    // to silence warning
    if(_y_array.size())
    {

    }

    //!\todo It can be used for Scatter plottings
}

void Display_container::set_display(const QVector<double> & _x_array,
                                    const QVector<double> & _y_array,
                                    bool replace, int after, bool symbols, bool line)
{
    // to silence warning
    if(_x_array.size() || _y_array.size() || symbols)
    {

    }

    //!\todo It can be used for Scatter plottings
}

std::shared_ptr< QVector<double> >  Display_container::get_x_values() const
{
    //! \todo Do something for raster data
    return nullptr;
}

std::shared_ptr< QVector<double> >  Display_container::get_y_values() const
{
    //! \todo Do something for raster data
    return nullptr;
}

void Display_container::clearAllPlotItems()
{
    QList<QwtPlotItem* > items = this->itemList(QwtPlotItem::Rtti_PlotCurve);
    for (int i = 0; i < items.size(); ++i)
        items.at(i)->detach();

    this->replot();
}

bool Display_container::event( QEvent *ev )
{
        switch( ev->type() )
        {
        case QEvent::MouseButtonPress:
        {
            const QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>( ev );
            if(mouseEvent->buttons() == Qt::LeftButton)
            {
              //  this->setCOG( mouseEvent->pos() );
            }
            if(mouseEvent->buttons() == Qt::MiddleButton)
            {
                //this->resetCOG();
            }

            break;
        }
        case QEvent::MouseMove:
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>( ev );
//            if(mouseEvent->buttons() == Qt::LeftButton)
//                move( mouseEvent->pos() );

            break;
        }
        case QEvent::MouseButtonRelease:
        {

            break;
        }
        default:
            break;
        }

        QwtPlot::event(ev);
    return true;
}
