#include "stackprocessor.h"
#include "ui_stackprocessor.h"

#include <stir/is_null_ptr.h>
#include <stir/IndexRange.h>
#include <stir/PixelsOnCartesianGrid.h>

#include <QFileInfo>

StackProcessor::StackProcessor(std::shared_ptr<Workspace> workspace,
                               QDialog *parent) :
    ImageProcessor(workspace, parent),
    ui(new Ui::StackProcessor)
{
    ui->setupUi(this);
    on_pushButton_clicked();
}

StackProcessor::~StackProcessor()
{
    delete ui;
}

void
StackProcessor::apply()
{

}

void StackProcessor::on_pushButton_clicked()
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

void StackProcessor::on_sld_slice_num_sliderMoved(int position)
{

}

void StackProcessor::on_psh_Apply_clicked()
{
    
    std::shared_ptr<stir::ArrayInterface> r = m_workspace_sptr->get_current_array_ptr();
    current_data_set_ptr = dynamic_cast<stir::Array<3, float> *> (r.get());

    if (ui->tabWidget->currentIndex() == 0)
        split_array();
    else if(ui->tabWidget->currentIndex() == 1)
        return;
    else if(ui->tabWidget->currentIndex() == 2)
        resort();
}

void StackProcessor::split_array()
{
    if (is_null_ptr(current_data_set_ptr))
        return;

    int num_poss = current_data_set_ptr->get_length();

    QString nameT = m_workspace_sptr->get_current_name();

    {
        QFileInfo info(nameT);
        if(info.isFile())
        {
            nameT = info.baseName();
        }
    }

    const stir::IndexRange<3> plane_size = current_data_set_ptr->get_index_range();

    int maxZ = 0;

    bool is_voxels = false;

    int split_pos = ui->sld_slice_num->value();

    int num_subsets = (static_cast<float>(num_poss) / split_pos);

    const stir::BasicCoordinate<3,int> min_coord = stir::make_coordinate(0, plane_size[1].get_min_index(), plane_size[2].get_min_index());
    stir::BasicCoordinate<3,int> max_coords = stir::make_coordinate(split_pos-1, plane_size[1].get_max_index(), plane_size[2].get_max_index());

    bool get_voxels = false;

    stir::VoxelsOnCartesianGrid<float>* tmpVG = dynamic_cast<stir::VoxelsOnCartesianGrid<float>* >(current_data_set_ptr);
    stir::CartesianCoordinate3D<float> origin(0.0, 0.0, 0.0);
    stir::BasicCoordinate<3, float> spacing = stir::make_coordinate<float>(1.0, 1.0, 1.0);
    if(!stir::is_null_ptr(tmpVG))
    {
        get_voxels = true;
        origin = tmpVG->get_origin();
        spacing = tmpVG->get_grid_spacing();
    }
    int minZ = 0;
    for (int i = 0; i < num_subsets; ++i)
    {
        QString cur_name = nameT + "_" + QString::number(i);
        minZ = i*split_pos;

        maxZ = minZ + split_pos;

        if (maxZ > num_poss)
        {
            break;
            //            split_pos -= (maxZ - num_poss);
            //            maxZ = num_poss;
            //            max_coords = stir::make_coordinate(split_pos, plane_size[1].get_max_index(), plane_size[2].get_max_index());
        }

        //        const stir::BasicCoordinate<3,int> n_min_coord = stir::make_coordinate(minZ, plane_size[1].get_min_index(), plane_size[2].get_min_index());
        //        stir::BasicCoordinate<3,int> n_max_coords = stir::make_coordinate(maxZ, plane_size[1].get_max_index(), plane_size[2].get_max_index());

        const stir::IndexRange<3> trange(min_coord, max_coords);
        //        const stir::IndexRange<3> nrange(n_min_coord, n_max_coords);

        std::shared_ptr<stir::VoxelsOnCartesianGrid<float> > new_image( new stir::VoxelsOnCartesianGrid<float>(trange,
                                                                                                               origin,
                                                                                                               spacing));


        savvy::fill_Array(*current_data_set_ptr,
                          *new_image,
                          minZ);

        m_workspace_sptr->append_to_workspace(new_image, cur_name);

    }
}

void StackProcessor::resort()
{
    if (is_null_ptr(current_data_set_ptr))
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

    if(ui->chk_invert_order->isChecked())
    {
        for (int i = 1; i <= num_poss; ++i)
        {
            int take_num = num_poss - i;

            stir::Array<2, float>  d = current_data_set_ptr->at(i-1);

            savvy::fill_Array(d,
                              *new_image,
                              take_num);
        }

        current_data_set_ptr = target;
    }

    if (ui->chk_human_sorting->isChecked())
    {
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
}



void StackProcessor::on_sld_slice_num_valueChanged(int value)
{
    ui->label_2->setText(QString::number(value));
    int num_datasets = ui->sld_slice_num->maximum();
    if (value > 0)
    {
        num_datasets = ceil(static_cast<float>(num_datasets) / value);
    }

    ui->label_3->setText(QString::number(num_datasets));
}
