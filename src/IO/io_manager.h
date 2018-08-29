#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "stir/common.h"
#include "stir/IO/interfile.h"
#include "stir/interfile_keyword_functions.h"
#include "stir/IO/ExamData.h"
#include "stir/Array.h"
#include "stir/DiscretisedDensity.h"
#include "stir/DiscretisedDensityOnCartesianGrid.h"
#include "stir/VoxelsOnCartesianGrid.h"
#include "stir/is_null_ptr.h"

#ifdef HAVE_ITK
    #include "stir/IO/FileSignature.h"
    #include "stir/IO/InputFileFormat.h"
    #include "stir/IO/ITKImageInputFileFormat.h"
#endif

#include <ctype.h>

namespace io_manager {

bool write_array(const std::string file_name,
                 const std::shared_ptr<stir::ArrayInterface>& array)
{

    // try to see if we are lucky and the array is VoxelsOnCartesianGrid
    {
        stir::VoxelsOnCartesianGrid<float>* t = dynamic_cast<stir::VoxelsOnCartesianGrid<float>*>( array.get() );
        if(!stir::is_null_ptr(t))
        {
            if(stir::write_basic_interfile(file_name, *t) == stir::Succeeded::yes)
                return true;
            else
                return false;
        }
    }

    return false;
}

//! Open file magic.
//! This function depends on stir IO library and is optimised for
//! openning interfiles and optionally DICOM files (if the dependency
//! is built).
int open_array(const std::string& file_name,
                std::shared_ptr<stir::ArrayInterface>& array)
{

    stir::FileSignature f(file_name.c_str());

    if (stir::is_interfile_signature(f.get_signature()))
    {
        array.reset( new stir::VoxelsOnCartesianGrid<float> ( *stir::read_interfile_image(file_name.c_str())) );
        return 1;
    }
#ifdef HAVE_ITK
    else
    {
        stir::InputFileFormat<stir::DiscretisedDensity<3, float> >* d = new stir::ITKImageInputFileFormat();

        if(d->can_read(f, file_name.c_str()))
        {
            std::unique_ptr<stir::DiscretisedDensity<3, float> > ddu = d->read_from_file(file_name.c_str());
            array = std::move(ddu);
            return 1;
        }
    }
#endif

    return 0;
}


//! Return the header of the file with file_name
//! \todo NIY (Not implemented yet)
QString open_header(std::string file_name)
{
    // to silence warning
    if(file_name.size())
    {
//stir::read_interfile_image_header()
    }

	return QString("");
}


}

#endif
