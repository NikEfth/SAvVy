#include "display_container_2d.h"


Display_container_2d::Display_container_2d(QWidget *parent) : Display_container(parent)
{

}

float Display_container_2d::at(int col, int row) const
{
    return data[col][row];
}

void Display_container_2d::set_array(stir::Array<2, float>* _array)
{
    data.resize(_array->get_index_range());
    data = *_array;
}

void Display_container_2d::set_array(const stir::Array<2, float> &_array)
{
    data.resize(_array.get_index_range());
    data = _array;
}

stir::Array<2, float>::full_iterator
Display_container_2d::get_beging()
{
    return data.begin_all();
}

stir::Array<2, float>::const_full_iterator
Display_container_2d::get_beging_const() const
{
    return data.begin_all_const();
}

stir::Array<2, float>::full_iterator
Display_container_2d::get_end()
{
    return data.end_all();
}

stir::Array<2, float>::const_full_iterator
Display_container_2d::get_end_const() const
{
    return data.end_all_const();
}


/*
 *
 *  TESTS
 *
 *
 */

//bool Display_container::check_data_ranges()
//{
////    stir::Array<3, float>::const_iterator it = data->begin();


//}

//bool Display_container::check_data_ranges_all()
//{
////    stir::Array<3, float>::const_full_iterator it = data->begin_all_const();

////    float test;
////    for ( ; it != data->end_all_const(); ++it)
////        test = *it;

//}

