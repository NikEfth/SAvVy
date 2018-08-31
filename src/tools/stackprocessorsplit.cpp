#include "stackprocessorsplit.h"

#include <stir/IndexRange.h>
#include <stir/PixelsOnCartesianGrid.h>

StackProcessorSplit::StackProcessorSplit(stir::Array<3, float> *data_sptr,
                                         QObject *parent) :
    StackProcessor (data_sptr, parent)
{

}

void StackProcessorSplit::set_split_slide(int val)
{
    if (val <= m_data_ptr->get_length())
        split_pos = val;
}

void StackProcessorSplit::set_recrusive(bool val)
{
    recrusive = val;
}

bool StackProcessorSplit::execute()
{

    int num_poss = m_data_ptr->get_length();

    int num_subsets = 2;

    if(recrusive)
        num_subsets = static_cast<float>(num_poss) / split_pos;

    const stir::IndexRange<3> plane_size = m_data_ptr->get_index_range();

    const stir::BasicCoordinate<3,int> min_coord = stir::make_coordinate(0, plane_size[1].get_min_index(), plane_size[2].get_min_index());
    stir::BasicCoordinate<3,int> max_coords = stir::make_coordinate(split_pos-1, plane_size[1].get_max_index(), plane_size[2].get_max_index());

    stir::VoxelsOnCartesianGrid<float>* tmpVG = dynamic_cast<stir::VoxelsOnCartesianGrid<float>* >(m_data_ptr);

    stir::CartesianCoordinate3D<float> origin(0.0, 0.0, 0.0);
    stir::BasicCoordinate<3, float> spacing = stir::make_coordinate<float>(1.0, 1.0, 1.0);

    if(!stir::is_null_ptr(tmpVG))
    {
        origin = tmpVG->get_origin();
        spacing = tmpVG->get_grid_spacing();
    }

    int maxZ = 0;
    int minZ = 0;

    for (int i = 0; i < num_subsets; ++i)
    {

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
        savvy::fill_Array(*m_data_ptr,
                          *new_image,
                          minZ);

        res.append(new_image);
    }

    return num_subsets;
}

std::shared_ptr<stir::VoxelsOnCartesianGrid<float> > StackProcessorSplit::get_slit_array(int i) const
{
    if (i < res.size())
        return res.at(i);

    return nullptr ;
}
