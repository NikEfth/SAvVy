#include "panel_opened_files.h"
#include "ui_panel_opened_files.h"

Panel_opened_files::Panel_opened_files(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel_opened_files)
{
    ui->setupUi(this);
}

Panel_opened_files::~Panel_opened_files()
{
    delete ui;
}

qint16 Panel_opened_files::findQListWidgetItem(QListWidget* this_list,
                                               const QString& _id)
{
    for (qint16 i = 0; i < this_list->count(); ++i)
        if (this_list->item(i)->toolTip() == _id)
            return i;

    return -1;
}

void Panel_opened_files::appendToOpenedList(Display_container* child)
{

    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setToolTip( QString::number(child->get_my_id()));
    tmp_itm->setText(child->get_file_name());

    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable);
    tmp_itm->setCheckState(Qt::Unchecked);

    ui->listOpenedFiles->addItem(tmp_itm);
    ui->listOpenedFiles->setCurrentItem(tmp_itm);

    //    mathMan->updateOpenedFiles();
}

void Panel_opened_files::removeFromOpenedList(Display_container* child)
{
    // Get sender's row number
    qint16 rowNum = findQListWidgetItem(ui->listOpenedFiles,
                                        QString::number(child->get_my_id()));

    if (rowNum >= 0)
    {
        ui->listOpenedFiles->takeItem(rowNum); //row
        // Should delete ... but it is getting destroyed soon.
    }
    // Activate the window which is before the closed. - Feels cool.
    //    if(ui->listOpenedFiles->count()>0)
    //    {
    //        QString this_label = ui->listOpenedFiles->currentItem()->toolTip();

    //        if (QMdiSubWindow *existing = findMdiChild(this_label)) {
    //            ui->mdiArea->setActiveSubWindow(existing);
    //        }
    //}
}
