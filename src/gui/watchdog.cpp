#include "watchdog.h"

WatchDog::WatchDog(QWidget *parent) : QWidget(parent)
{



}

WatchDog::~WatchDog()
{

}

//void WatchDog::setContainer(DisplayInterface *_s)
//{
//    if(stir::is_null_ptr(m_current_container_ptr))
//    {
//        m_current_container_ptr = _s;
//        emit container_set();
//        //        my_histogram->setEnabled(true);
//        //        my_histogram->setScreen(sc);
//        //        connect(m_current_container_ptr, &Display_container::activeScreenUpdated,
//        //                my_histogram, &BarScreen::replot_me);

//        //        updateMarkers(my_histogram->getMinMarker(), my_histogram->getMaxMarker());
//        //        my_histogram->setNumBin(ui->spinBox->text().toInt());
//        //        my_histogram->setCutOff(ui->doubleSpinBox->value());
//        //        // Get the initial values.
//        //        ui->le_max->setText(QString::number(my_histogram->getMaxMarker()));
//        //        ui->le_min->setText(QString::number(my_histogram->getMinMarker()));
//        //        ui->le_win->setText(QString::number(my_histogram->getWindow()));
//        //        ui->le_level->setText(QString::number(my_histogram->getLevel()));
//    }
//}

//void WatchDog::unsetContainer()
//{
//    disconnect(m_current_container_ptr);
//    m_current_container_ptr = nullptr;
//    emit container_unset();
//}
