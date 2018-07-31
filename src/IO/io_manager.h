#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "stir/common.h"
#include "stir/IO/interfile.h"
#include "stir/IO/ExamData.h"
#include "stir/Array.h"
#include "stir/VoxelsOnCartesianGrid.h"

namespace io_manager {

//!
//! \brief open_file_magic
//! \param file_name
//! \return
//!
int open_array(const std::string& file_name,
                std::shared_ptr<stir::ArrayInterface>& array)
{

    if (!stir::is_interfile_signature(file_name.c_str()))
    {
        array.reset( new stir::VoxelsOnCartesianGrid<float> ( *stir::read_interfile_image(file_name.c_str())) );
        return 1;
    }

    return 0;
}

void open_header(std::string file_name)
{
    // to silence warning
    if(file_name.size())
    {

    }
}


}

#endif
