#include "ctrl_levels.h"
#include "ui_ctrl_levels.h"

CTRL_Levels::CTRL_Levels(QWidget *parent) :
    WatchDog (parent),
    ui(new Ui::CTRL_Levels)
{
    ui->setupUi(this);

    my_histogram = new Histogram_manager(0, 0, ui->groupBox_2);
    my_histogram->set_no_controls(false);
    ui->gridLayout_3->addWidget(my_histogram);

    //    connect(my_histogram, &BarScreenWL::markersUpdated, this, &ContrastMan::updateMarkers);
    //    connect(my_histogram, &BarScreenWL::WL, this, &ContrastMan::updateWL);
}

CTRL_Levels::~CTRL_Levels()
{
    delete ui;
}

void CTRL_Levels::setContainer(DisplayInterface *_s)
{
    if(stir::is_null_ptr(m_current_container_ptr))
    {
        m_current_container_ptr = _s;
        my_histogram->setEnabled(true);
        my_histogram->get_display()->linkTo(m_current_container_ptr->get_display());
        m_current_container_ptr->get_display()->linkTo(my_histogram->get_display());

        connect(m_current_container_ptr->get_display().get(), &Display_container::display_updated,
                this, &CTRL_Levels::display_updated);

        my_histogram->get_display()->update_display();
        //        updateMarkers(my_histogram->getMinMarker(), my_histogram->getMaxMarker());

        // Get the initial values.
        m_current_container_ptr->get_display()->get_min_max(m_min_value, m_max_value);
        updateMarkers(m_min_value, m_max_value);
    }
}

void CTRL_Levels::unsetContainer()
{
    if (!stir::is_null_ptr(m_current_container_ptr))
    {
        disconnect(m_current_container_ptr->get_display().get(), &Display_container::display_updated,
                this, &CTRL_Levels::display_updated);
        my_histogram->get_display()->unLink();
        m_current_container_ptr->get_display()->unLink();
        m_current_container_ptr = nullptr;
    }
    my_histogram->setEnabled(false);
    //    my_histogram->unsetScreen(false);
}

void CTRL_Levels::display_updated()
{
    m_current_container_ptr->get_display()->get_min_max(m_min_value, m_max_value);
    updateMarkers(m_min_value, m_max_value);
    m_window = m_max_value - m_min_value;
    m_level  = (m_min_value + m_max_value) * 0.5;
    updateWL(m_window, m_level);

}

void CTRL_Levels::updateMarkers(const double _min, const double _max)
{
    m_min_value = _min;
    m_max_value = _max;
    ui->le_min->setText(QString::number(m_min_value));
    ui->le_max->setText(QString::number(m_max_value));

}

void CTRL_Levels::updateWL(const double window, const double lavel)
{
    ui->le_win->setText(QString::number(window));
    ui->le_level->setText(QString::number(lavel));
}

void CTRL_Levels::on_btn_reset_clicked()
{
    //     my_histogram->resetCOG(0);
}

void CTRL_Levels::on_btn_auto_clicked()
{
    //    my_histogram->resetCOG();
}

void CTRL_Levels::on_le_max_returnPressed()
{
    //        bool ok;
    //        float f = ui->le_max->text().toFloat(&ok);
    //        if (ok)
    //            my_histogram->setMaxMarker(f);
}

void CTRL_Levels::on_le_min_returnPressed()
{
    //    bool ok;
    //    float f = ui->le_min->text().toFloat(&ok);
    //    if (ok)
    //        my_histogram->setMinMarker(f);
}

void CTRL_Levels::on_le_level_returnPressed()
{
    //    bool ok;
    //    float f = ui->le_level->text().toFloat(&ok);
    //    if (ok)
    //        my_histogram->setLevel(f);
}

void CTRL_Levels::on_le_win_returnPressed()
{
    //    bool ok;
    //    float f = ui->le_win->text().toFloat(&ok);
    //    if (ok)
    //        my_histogram->setWindow(f);
}
