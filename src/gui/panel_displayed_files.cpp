#include "panel_displayed_files.h"
#include "ui_panel_displayed_files.h"

Panel_displayed_files::Panel_displayed_files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_displayed_files)
{
    ui->setupUi(this);

    connect(ui->listOpenedFiles, &QListWidget::itemChanged,
            this, &Panel_displayed_files::highlightChecked);
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
}

void Panel_displayed_files::appendToOpenedList(Display_container* child)
{

    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setToolTip( QString::number(child->get_my_id()));
    tmp_itm->setText(child->get_file_name());

    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    tmp_itm->setCheckState(Qt::Unchecked);

    ui->listOpenedFiles->addItem(tmp_itm);
    ui->listOpenedFiles->setCurrentItem(tmp_itm);

}

void Panel_displayed_files::appendToOpenedList(Display_manager* child)
{

    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setToolTip( QString::number(child->get_my_id()));
    tmp_itm->setText(child->get_file_name());

    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    tmp_itm->setCheckState(Qt::Unchecked);

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
