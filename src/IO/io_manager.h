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
stir::ArrayInterface* open_array(std::string file_name)
{

    if (!stir::is_interfile_signature(file_name.c_str()))
    {
        return stir::read_interfile_image(file_name.c_str());
    }

    return nullptr;
}

void open_header(std::string file_name)
{

}


}

#endif
