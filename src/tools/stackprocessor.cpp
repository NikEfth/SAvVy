#include "stackprocessor.h"

StackProcessor::StackProcessor(stir::Array<3, float>* data_ptr,
                               QObject *parent) :
    QObject(parent),
    m_data_ptr(data_ptr)
{

}
