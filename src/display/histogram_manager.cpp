#include "histogram_manager.h"
#include "ui_histogram_manager.h"

Histogram_manager::Histogram_manager(int _my_id, int _num_dim, QWidget *parent) :
    DisplayInterface(_my_id, _num_dim, parent),
    ui(new Ui::Histogram_manager)
{
    ui->setupUi(this);
    _histogram = new Display_container_bar(0, 0, this);

    ui->verticalLayout_3->addWidget(_histogram);
}

Histogram_manager::~Histogram_manager()
{
    delete ui;
}

void Histogram_manager::on_spinBox_valueChanged(int arg1)
{
    _histogram->setNumBin_update(arg1);
}

void Histogram_manager::on_doubleSpinBox_valueChanged(double arg1)
{
    _histogram->setCutOff_update(static_cast<float>(arg1));
}

void Histogram_manager::set_display(void*_in)
{
    _histogram->set_display(_in);
}

void Histogram_manager::set_display(std::shared_ptr<QVector<double> > _in)
{
    _histogram->set_display(_in);
}

void Histogram_manager::setNumBin(const int& _n)
{
    _histogram->setNumBin(_n);
    ui->spinBox->blockSignals(true);
    ui->spinBox->setValue(_n);
    ui->spinBox->blockSignals(false);
}

void Histogram_manager::setCutOff(const float& _n)
{
    _histogram->setCutOff(_n);
    ui->doubleSpinBox->blockSignals(true);
    ui->doubleSpinBox->setValue(_n);
    ui->doubleSpinBox->blockSignals(false);
}
