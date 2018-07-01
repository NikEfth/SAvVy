#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "stir/common.h"
#include "stir/IO/interfile.h"
#include "stir/IO/ExamData.h"
#include "stir/Array.h"
#include "stir/VoxelsOnCartesianGrid.h"

#include <QVector>

namespace io_manager {

//!
//! \brief open_file_magic
//! \param file_name
//! \return
//!
stir::Array<3, float>* open_array(std::string file_name, int& num_of_data,
                                  int& x_dim,
                                  int& y_dim,
                                  int& z_dim)
{

    if (stir::is_interfile_signature(file_name.c_str()))
    {
        return stir::read_interfile_image(file_name.c_str());
    }

    return nullptr;
}

void open_header(std::string file_name)
{

}

void Array2QVector(const stir::Array<1, float> & input,
                   QVector<double> & output)
{

    if (output.size() == 0)
        output.resize(input.size());
    assert(input.get_length() == output.size());

    stir::Array<1, float>::const_full_iterator it = input.begin_all_const();
    QVector<double>::iterator ot = output.begin();

    for (; it != input.end_all(); ++it, ++ot)
    {
        *ot = static_cast<double>(*it);
    }
}

}

#endif
