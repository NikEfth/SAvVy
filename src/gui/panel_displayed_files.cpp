#include "panel_displayed_files.h"
#include "ui_panel_displayed_files.h"

#include <QFileDialog>
#include <QPrinter>
#include <qwt_plot_renderer.h>
#include <QSettings>

Panel_displayed_files::Panel_displayed_files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_displayed_files)
{
    ui->setupUi(this);

    QSettings settings;

    connect(ui->listOpenedFiles, &QListWidget::itemChanged,
            this, &Panel_displayed_files::highlightChecked);

    if(settings.contains("ColorMapsPath") && settings.contains("defaultColorMap"))
        _colormaps = new ColorMap(settings.value("ColorMapsPath").toString(),
                                  settings.value("defaultColorMap").toString());
    else if (settings.contains("ColorMapsPath"))
        _colormaps = new ColorMap(settings.value("ColorMapsPath").toString(),
                                  "BW");
    else
        _colormaps = new ColorMap();

    pnl_opened_file_controls = new Panel_opened_file_controls(_colormaps->getColormapList(), this);
    connect(pnl_opened_file_controls, &Panel_opened_file_controls::colormap_changed,
            this, &Panel_displayed_files::on_set_colormap);
    pnl_opened_file_controls->show_panel();

    ui->verticalLayout->addWidget(pnl_opened_file_controls);
}

Panel_displayed_files::~Panel_displayed_files()
{
    delete ui;
}

qint16 Panel_displayed_files::findQListWidgetItem(const QString& _id)
{
    for (qint16 i = 0; i <  ui->listOpenedFiles->count(); ++i)
        if ( ui->listOpenedFiles->item(i)->toolTip() == _id)
            return i;

    return -1;
}

void Panel_displayed_files::set_active(int _id)
{
    ui->listOpenedFiles->setCurrentRow(findQListWidgetItem(QString::number(_id)));

    Display_manager* t = get_active();

    if(stir::is_null_ptr(t))
        return;

    updateGUI(t);
}

void Panel_displayed_files::updateGUI(Display_manager* _cur)
{
    pnl_opened_file_controls->show_panel(_cur->get_num_dimensions());
}

Display_manager* Panel_displayed_files::get_active()
{
    int ind = ui->listOpenedFiles->currentRow();

    if (ind == -1)
        return nullptr;

    bool ok;
    unsigned int name = ui->listOpenedFiles->item(ind)->toolTip().toUInt(&ok);

    if (!ok)
        return nullptr;

    Display_manager* ret = nullptr;
    ret = displs.value(name);

    return ret;
}

void Panel_displayed_files::appendToOpenedList(Display_container* child)
{
    // to silence warning
    if(child)
    {

    }

    //! \todo Display_container cannot be used for display outside of a wrapper
    //    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    //    tmp_itm->setToolTip( QString::number(child->get_my_id()));
    //    tmp_itm->setText(child->get_file_name());

    //    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    //    tmp_itm->setCheckState(Qt::Unchecked);

    //    ui->listOpenedFiles->addItem(tmp_itm);
    //    ui->listOpenedFiles->setCurrentItem(tmp_itm);

}

void Panel_displayed_files::appendToOpenedList(Display_manager* child, unsigned int _id)
{

    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setToolTip( QString::number(child->get_my_id()));
    tmp_itm->setText(child->get_file_name());

    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    tmp_itm->setCheckState(Qt::Unchecked);

    displs.insert(_id, child);

    child->set_color_map(_colormaps->getDefaultColorMap());

    ui->listOpenedFiles->addItem(tmp_itm);
    ui->listOpenedFiles->setCurrentItem(tmp_itm);

}

void Panel_displayed_files::rename(const QString& _id, QString _new_name)
{
    // Get sender's row number
    qint16 rowNum = findQListWidgetItem(_id);
    ui->listOpenedFiles->blockSignals(true);
    ui->listOpenedFiles->item(rowNum)->setText(_new_name);
    ui->listOpenedFiles->blockSignals(false);
}

void Panel_displayed_files::rename(const int& _id, QString _new_name)
{
    // Get sender's row number
    qint16 rowNum = findQListWidgetItem(QString::number(_id));
    ui->listOpenedFiles->item(rowNum)->setText(_new_name);
}

void Panel_displayed_files::removeFromOpenedList(DisplayInterface* child)
{
    // Get sender's row number
    qint16 rowNum = findQListWidgetItem(QString::number(child->get_my_id()));

    if (rowNum >= 0)
    {
        ui->listOpenedFiles->takeItem(rowNum); //row
        // Should delete ... but it is getting destroyed soon.
    }
}

void Panel_displayed_files::on_listOpenedFiles_itemDoubleClicked(QListWidgetItem *item)
{
    emit double_clicked_item(item->toolTip());
}

void Panel_displayed_files::highlightChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
    {
        ui->listOpenedFiles->blockSignals(true);
        item->setBackgroundColor(QColor("#ffffb2"));
        ui->listOpenedFiles->blockSignals(false);

        grouped_windows.append(item->toolTip());
    }
    else
    {
        ui->listOpenedFiles->blockSignals(true);
        item->setBackgroundColor(QColor("#ffffff"));
        ui->listOpenedFiles->blockSignals(false);

        grouped_windows.removeAll(item->toolTip());
    }
    ui->lbl_num_groupped->setText(QString::number(grouped_windows.size()));
}

bool Panel_displayed_files::has_grouped_items() const
{
    return grouped_windows.size()>0 ? true: false;
}

QVector<QString> Panel_displayed_files::get_groupped() const
{
    return grouped_windows;
}

void Panel_displayed_files::on_psh_print_image_clicked()
{

    int currentItem = ui->listOpenedFiles->currentRow();

    if (currentItem == -1)
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save vector"), QDir::home().absolutePath(), tr("Scalable images (*.svg)"));
    if (fileName.size() == 0)
        return;

    Display_manager* dsp = get_active();

    QwtPlotRenderer renderer;

//    dsp->get_display()->setAutoFillBackground( true );
    QPalette def = this->palette();
    QPalette p = def;
    p.setColor(QPalette::Window, Qt::white);
    dsp->get_display()->setPalette(p);

    renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );

    renderer.renderDocument(dsp->get_display().get(), fileName, QSizeF(dsp->parentWidget()->width(),
                                                                     dsp->parentWidget()->height()));

    dsp->get_display()->setPalette(def);

}

void Panel_displayed_files::on_psh_save_image_clicked()
{
    int currentItem = ui->listOpenedFiles->currentRow();

    if (currentItem == -1)
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save vector"), QDir::home().absolutePath(), tr("Scalable images (*.svg)"));
    if (fileName.size() == 0)
        return;

    Display_manager* dsp = get_active();

    QwtPlotRenderer renderer;

    dsp->get_display()->setAutoFillBackground( true );
    QPalette def = dsp->get_display()->palette();
    QPalette p = def;
    p.setColor(QPalette::Window, Qt::white);
    dsp->get_display()->setPalette(p);

    renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );

    qint32 w = dsp->get_display()->width();
    qint32 h = dsp->get_display()->height();
    QPixmap pixmap(w, h);

    renderer.renderTo(dsp->get_display().get(), pixmap);

    if(fileName.contains(".png"))
        pixmap.save(fileName, "PNG");
    else if(fileName.contains("jpg"))
        pixmap.save(fileName,"JPG");
    else if(fileName.contains("gif"))
        pixmap.save(fileName,"GIF");

    this->setPalette(def);
}

void Panel_displayed_files::on_set_colormap(int i)
{
    Display_manager* active = get_active();
    if(!stir::is_null_ptr(active))
        active->set_color_map( _colormaps->getColorMap(i));
}

void Panel_displayed_files::on_psh_zoom_in_clicked()
{
    int currentItem = ui->listOpenedFiles->currentRow();

    if (currentItem == -1)
        return;

     Display_manager* dsp = get_active();

     QSize cur_size = dsp->parentWidget()->size();

     dsp->parentWidget()->setFixedSize(cur_size.width()*2, cur_size.height()*2);
}

void Panel_displayed_files::on_psh_zoom_out_clicked()
{
    int currentItem = ui->listOpenedFiles->currentRow();

    if (currentItem == -1)
        return;

     Display_manager* dsp = get_active();

     QSize cur_size = dsp->parentWidget()->size();

     dsp->parentWidget()->setFixedSize(cur_size.width()/2, cur_size.height()/2);
}
