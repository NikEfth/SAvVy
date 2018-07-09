#include "workspace.h"
#include "ui_workspace.h"

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

void Workspace::append_to_openedFiles(const QString& _id)
{
    QListWidgetItem* tmp_itm =  new QListWidgetItem();

    tmp_itm->setText(_id);
    tmp_itm->setFlags(tmp_itm->flags() | Qt::ItemIsUserCheckable  | Qt::ItemIsEditable);
    tmp_itm->setCheckState(Qt::Unchecked);

    ui->listOpenedFiles->addItem(tmp_itm);
    ui->listOpenedFiles->setCurrentItem(tmp_itm);

    updateGUI();
}

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

        grouped_files.append(item->toolTip());
    }
    else
    {
        ui->listOpenedFiles->blockSignals(true);
        item->setBackgroundColor(QColor("#ffffff"));
        ui->listOpenedFiles->blockSignals(false);

        grouped_files.removeAll(item->toolTip());
    }
    ui->lbl_num_groupped->setText(QString::number(grouped_files.size()));
}

bool Workspace::has_grouped_items() const
{
    return grouped_files.size()>0 ? true: false;
}

QVector<QString> Workspace::get_groupped() const
{
    return grouped_files;
}


void Workspace::on_display_array_pressed()
{
    emit display_current( openned_files[ui->listOpenedFiles->currentRow()],
            get_array_name(ui->listOpenedFiles->currentRow()) );
}

void Workspace::on_duplicate_array_clicked()
{
    if (openned_files.size() == 0)
        return;

    int current_row  = ui->listOpenedFiles->currentRow();
    int dims = openned_files.at(current_row)->get_num_dimensions();

    if (dims == 1)
    {
        stir::Array<1,float>* tmp = dynamic_cast<stir::Array<1,float>* >(openned_files[current_row].get());

        std::shared_ptr<stir::ArrayInterface> wtmp(
                new stir::Array<1,float>( *tmp ));

        append_to_workspace(wtmp, "copyOf_" + ui->listOpenedFiles->item(current_row)->text() );

    }
    else if (dims == 2)
    {
        stir::Array<2,float>* tmp = dynamic_cast<stir::Array<2,float>* >(openned_files[current_row].get());

        std::shared_ptr<stir::ArrayInterface> wtmp(
                new stir::Array<2,float>( *tmp ));

        append_to_workspace(wtmp, "copyOf_" + ui->listOpenedFiles->item(current_row)->text() );
    }

    updateGUI();
}
