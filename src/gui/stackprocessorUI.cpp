#include "stackprocessorUI.h"
#include "ui_stackprocessorUI.h"

#include <stir/is_null_ptr.h>

#include <QFileInfo>

#include "src/tools/stackprocessorsplit.h"

StackProcessorUI::StackProcessorUI(std::shared_ptr<Workspace> workspace,
                                   QDialog *parent):
    ui(new Ui::StackProcessorUI),
    m_workspace_sptr(workspace)
{
    ui->setupUi(this);
    on_pushButton_clicked();

    connect(m_workspace_sptr.get(), &Workspace::refresh,
            this, &StackProcessorUI::refresh_dataset);
}

StackProcessorUI::~StackProcessorUI()
{
    disconnect();
    delete ui;
}

void StackProcessorUI::on_pushButton_clicked()
{
    std::shared_ptr<stir::ArrayInterface> r = m_workspace_sptr->get_current_array_ptr();

    if(r != nullptr)
    {
        if(r->get_num_dimensions() < 3)
        {
            no_stack = true;
        }
        else
        {
            no_stack = false;
        }
    }
    else
        no_stack = true;

    ui->tabWidget->setEnabled(!no_stack);

    if(!no_stack)
    {
        current_data_set_ptr = dynamic_cast<stir::Array<3, float> *> (r.get());

        ui->sld_slice_num->setMaximum(current_data_set_ptr->get_length());
    }
}

bool StackProcessorUI::refresh_dataset()
{
    
    std::shared_ptr<stir::ArrayInterface> r = m_workspace_sptr->get_current_array_ptr();

    if(stir::is_null_ptr(r))
        return false;

    current_data_set_ptr = dynamic_cast<stir::Array<3, float> *> (r.get());

    if(stir::is_null_ptr(current_data_set_ptr))
        return false;

    return true;
}

void StackProcessorUI::split_array()
{
    if (is_null_ptr(current_data_set_ptr))
        return;

    QString nameT = m_workspace_sptr->get_current_name();

    {
        QFileInfo info(nameT);
        if(info.isFile())
        {
            nameT = info.baseName();
        }
    }

    int slit_pos = ui->sld_slice_num->value();

    StackProcessorSplit splt(current_data_set_ptr, this);
    splt.set_split_slide(slit_pos);
    splt.set_recrusive(ui->chk_recrusive->isChecked());
    splt.execute();

    int num_datasets = ui->sld_slice_num->maximum();

    if (slit_pos > 0 && ui->chk_recrusive->isChecked())
        num_datasets = static_cast<float>(num_datasets) / slit_pos;
    else
        num_datasets = 2;

    for (int i = 0; i < num_datasets; ++i)
    {
        QString cur_name = nameT + "_" + QString::number(i);
        std::shared_ptr<stir::VoxelsOnCartesianGrid<float> > new_image = splt.get_slit_array(i);
        m_workspace_sptr->append_to_workspace(new_image, cur_name);
    }

}

void StackProcessorUI::on_sld_slice_num_valueChanged(int value)
{
    ui->label_2->setText(QString::number(value));
    int num_datasets = ui->sld_slice_num->maximum();
    if (value > 0 && ui->chk_recrusive->isChecked())
        num_datasets = static_cast<float>(num_datasets) / value;
    else
        num_datasets = 2;

    ui->label_3->setText(QString::number(num_datasets));
}


void StackProcessorUI::on_psh_split_on_slide_clicked()
{
    if(refresh_dataset())
    {
        split_array();
    }
}

void StackProcessorUI::on_psh_reverse_order_clicked()
{
    if(!refresh_dataset())
        return;

    QString nameT = m_workspace_sptr->get_current_name();

    {
        QFileInfo info(nameT);
        if(info.isFile())
            nameT = "resorted_" + info.fileName();
    }

    std::shared_ptr<stir::ArrayInterface> new_image = m_workspace_sptr->get_new_empty_copy_current(nameT);

    stir::Array<3, float> * target = dynamic_cast<stir::Array<3, float> *> (new_image.get());

    int num_poss = current_data_set_ptr->get_length();

    for (int i = 1; i <= num_poss; ++i)
    {
        int take_num = num_poss - i;

        stir::Array<2, float>  d = current_data_set_ptr->at(i-1);

        savvy::fill_Array(d,
                          *new_image,
                          take_num);
    }
}

void StackProcessorUI::on_psh_human_order_clicked()
{
    if(!refresh_dataset())
        return;

    QString nameT = m_workspace_sptr->get_current_name();

    {
        QFileInfo info(nameT);
        if(info.isFile())
            nameT = "resorted_" + info.fileName();
    }

    std::shared_ptr<stir::ArrayInterface> new_image = m_workspace_sptr->get_new_empty_copy_current(nameT);

    stir::Array<3, float> * target = dynamic_cast<stir::Array<3, float> *> (new_image.get());

    int num_poss = current_data_set_ptr->get_length();

    std::vector<int> natural_order(num_poss);
    for (int i = 0; i < num_poss; ++i)
        natural_order[i] = i;

    QStringList a_order;
    for (int i = 0; i < num_poss; ++i)
    {
        a_order.push_back(QString::number(natural_order.at(i)));
    }

    QVector<int> re_natural_order(num_poss);

    a_order.sort();

    bool ok;
    for (int i = 0; i < num_poss; ++i)
    {
        int take_num = a_order.at(i).toInt(&ok);
        re_natural_order[i] = take_num;
    }

    for (int i = 0; i < num_poss; ++i)
    {
        int indx = re_natural_order.indexOf(i);
        if (indx < 0)
            break;

        stir::Array<2, float>  d = current_data_set_ptr->at(indx);

        savvy::fill_Array(d,
                          *new_image,
                          i);
    }

}

void StackProcessorUI::on_psh_concacenate_clicked()
{
    unsigned long int num_items = m_workspace_sptr->get_num_of_groupped_files();

    if (num_items <= 1)
        return;

    m_workspace_sptr->go_to_top();

    int combined_size = 0;

    std::shared_ptr<stir::ArrayInterface> array;

    bool first_run = true;

    while(m_workspace_sptr->get_next_item_in_group(
              array) != -1)
    {
        stir::Array<3,float> * r = dynamic_cast<stir::Array<3, float> *>(array.get());

        if(stir::is_null_ptr(r))
            continue;

        combined_size += r->get_length();

        first_run = false;
    }

    combined_size -= 1;

    stir::VoxelsOnCartesianGrid<float>* tmpVG = dynamic_cast<stir::VoxelsOnCartesianGrid<float>* >(array.get());
    stir::IndexRange<3> plane_size;
    stir::CartesianCoordinate3D<float> origin(0.0, 0.0, 0.0);
    stir::BasicCoordinate<3, float> spacing = stir::make_coordinate<float>(1.0, 1.0, 1.0);

    if(!stir::is_null_ptr(tmpVG))
    {
        origin = tmpVG->get_origin();
        spacing = tmpVG->get_grid_spacing();
        plane_size = tmpVG->get_index_range();
    }
    else
    {
        stir::Array<3, float>* tmpVG = dynamic_cast<stir::Array<3, float>* >(array.get());

        if(!stir::is_null_ptr(tmpVG))
        {
            plane_size = tmpVG->get_index_range();
        }
        else
            return;
    }

    const stir::BasicCoordinate<3,int> min_coord = stir::make_coordinate(0, plane_size[1].get_min_index(), plane_size[2].get_min_index());
    stir::BasicCoordinate<3,int> max_coords = stir::make_coordinate(combined_size, plane_size[1].get_max_index(), plane_size[2].get_max_index());
    stir::IndexRange<3> trange(min_coord, max_coords);

    std::shared_ptr<stir::ArrayInterface > new_array = m_workspace_sptr->get_new_empty_array("merged",
                                                                                             trange,
                                                                                             origin,
                                                                                             spacing);

    int cur_pos = 0;
    m_workspace_sptr->go_to_top();

    while(m_workspace_sptr->get_next_item_in_group(
              array) != -1)
    {
        cur_pos = savvy::fill_Array(*array, *new_array, cur_pos);
    }
}
