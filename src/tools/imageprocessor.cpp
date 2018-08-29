#include "imageprocessor.h"

ImageProcessor::ImageProcessor(std::shared_ptr<Workspace> workspace,
                               QDialog *parent) :
    QDialog(parent),
    m_workspace_sptr(workspace)
{

}
