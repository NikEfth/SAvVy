#include "workspace.h"
#include "ui_workspace.h"

#include "src/IO/io_manager.h"

#include "stir/is_null_ptr.h"

Workspace::Workspace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Workspace),
    next_dataset_id(0)
{
    ui->setupUi(this);

    connect(ui->listOpenedFiles, &QListWidget::itemChanged,
            this, &Workspace::highlightChecked);
}

Workspace::~Workspace()
{
    delete ui;
}

void Workspace::updateGUI()
{
    bool hasData= (openned_files.size() != 0);
    ui->remove_array->setEnabled(hasData);
    ui->display_array->setEnabled(hasData);
    ui->duplicate_array->setEnabled(hasData);
}


void Workspace::append_to_workspace(std::shared_ptr<stir::ArrayInterface> child,
                                    const QString& _name)
{

    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setText(_name);
    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    tmp_itm->setCheckState(Qt::Unchecked);

    ui->listOpenedFiles->addItem(tmp_itm);
    ui->listOpenedFiles->setCurrentItem(tmp_itm);

    openned_files.append(child);
    updateGUI();
}

//void Workspace::append_to_openedFiles(const QString& _id)
//{
//    QListWidgetItem* tmp_itm =  new QListWidgetItem();

//    tmp_itm->setText(_id);
//    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable  | Qt::ItemIsEditable);
//    tmp_itm->setCheckState(Qt::Unchecked);

//    ui->listOpenedFiles->addItem(tmp_itm);
//    ui->listOpenedFiles->setCurrentItem(tmp_itm);

//    updateGUI();
//}

void Workspace::remove_from_workspace(int _id)
{
    if (openned_files.size() == 0)
        return;

    openned_files.removeAt(_id);
    ui->listOpenedFiles->takeItem(_id);

    updateGUI();
}

void Workspace::on_remove_array_pressed()
{
    int current_row = ui->listOpenedFiles->currentRow();
    remove_from_workspace(current_row);
}

std::shared_ptr<stir::ArrayInterface>
Workspace::get_array_ptr(int _i)
{
    if (_i < openned_files.size())
        return openned_files[_i];
	return nullptr;
}

std::shared_ptr<stir::ArrayInterface>
Workspace::get_array_ptr(const QString& _s)
{
    for (int i = 0; i < ui->listOpenedFiles->count(); ++i)
    {
        if (ui->listOpenedFiles->item(i)->text() == _s)
            return openned_files[i];
    }

    return nullptr;
}

std::shared_ptr<stir::ArrayInterface>
Workspace::get_current_array_ptr()
{
    return get_array_ptr(ui->listOpenedFiles->currentRow());
}

QString Workspace::get_array_name(int _i)
{
    if (_i < ui->listOpenedFiles->count())
        return ui->listOpenedFiles->item(_i)->text();
	return QString("");
}

QString Workspace::get_current_name()
{
    return ui->listOpenedFiles->item( ui->listOpenedFiles->currentRow())->text();
}


void Workspace::highlightChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
    {
        ui->listOpenedFiles->blockSignals(true);
        item->setBackgroundColor(QColor("#ffffb2"));
        ui->listOpenedFiles->blockSignals(false);
    }
    else
    {
        ui->listOpenedFiles->blockSignals(true);
        item->setBackgroundColor(QColor("#ffffff"));
        ui->listOpenedFiles->blockSignals(false);
    }
    //    ui->lbl_num_groupped->setText(QString::number(grouped_files.size()));
}

bool Workspace::has_grouped_items() const
{
    for (qint16 i = 0; i <  ui->listOpenedFiles->count(); ++i)
        if ( ui->listOpenedFiles->item(i)->checkState() == Qt::Checked)
            return true;
    return false;
}

QStringList Workspace::get_groupped() const
{
	QStringList ret; 
    //    return grouped_files;
	return ret; 
}

std::shared_ptr <stir::ArrayInterface> Workspace::open_array(const QString& fileName)
{

    stir::ArrayInterface* input = io_manager::open_array(fileName.toStdString());

    if(stir::is_null_ptr(input))
        return nullptr;

    std::shared_ptr<stir::ArrayInterface> wtmp;
    int dims = input->get_num_dimensions();

    switch (dims) {
    case 1:
    {
        stir::Array<1,float>* tmp = dynamic_cast<stir::Array<1,float>* >(input);
        wtmp.reset( new stir::Array<1,float>( *tmp ));
    }
        break;
    case 2:
    {
        stir::Array<2,float>* tmp = dynamic_cast<stir::Array<2,float>* >(input);
        wtmp.reset(new stir::Array<2,float>( *tmp ));
    }
        break;
    case 3:
    {
        stir::Array<3,float>* tmp = dynamic_cast<stir::Array<3,float>* >(input);
        wtmp.reset( new stir::Array<3,float>( *tmp ));
    }
        break;
    }

    append_to_workspace(wtmp,fileName);
    updateGUI();
    return wtmp;
}

void Workspace::on_display_array_pressed()
{
    if (has_grouped_items())
    {
        for (qint16 i = 0; i <  ui->listOpenedFiles->count(); ++i)
            if ( ui->listOpenedFiles->item(i)->checkState() == Qt::Checked)
                emit display_current( openned_files[i],
                                      get_array_name(i) );
    }
    else
        emit display_current( openned_files[ui->listOpenedFiles->currentRow()],
            get_array_name(ui->listOpenedFiles->currentRow()) );
}

void Workspace::on_duplicate_array_clicked()
{
    if (openned_files.size() == 0)
        return;

    int current_row  = ui->listOpenedFiles->currentRow();
    int dims = openned_files.at(current_row)->get_num_dimensions();
    std::shared_ptr<stir::ArrayInterface> wtmp;

    switch (dims) {
    case 1:
    {
        wtmp.reset( new stir::Array<1,float>( *dynamic_cast<stir::Array<1,float>* >(openned_files[current_row].get()) ));
    }
        break;
    case 2:
    {
        wtmp.reset( new stir::Array<2,float>( *dynamic_cast<stir::Array<2,float>* >(openned_files[current_row].get()) ));
    }
        break;
    case 3:
    {
        wtmp.reset( new stir::Array<3,float>( *dynamic_cast<stir::Array<3,float>* >(openned_files[current_row].get()) ));
    }
        break;
    }

    append_to_workspace(wtmp, "copyOf_" + ui->listOpenedFiles->item(current_row)->text() );
    updateGUI();
}
