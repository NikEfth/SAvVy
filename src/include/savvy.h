#ifndef SAVVY_H
#define SAVVY_H

#include <QVector>

#include "stir/common.h"
#include "stir/Array.h"
#include "stir/VoxelsOnCartesianGrid.h"

namespace savvy {

template<class T>
void serialize_QVector(const QVector<QVector<T> >& in,
                      QVector<T>& out)
{
    assert(in.size() > 0);
    int row_num = in.size();
    assert(in[0].size() > 0);
    int row_size = in[0].size();

    if(out.size() == 0)
    {
        out.resize(row_num*row_size);
    }

    int idx = 0;

    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
            out[idx] = in[i][j];
}

//!
//! \brief deserialize_QVector
//! \param in
//! \param out
//! \warning Be carefull that [XxY] = size_row_vector
template<class T>
void deserialize_QVector(const QVector<T>& in,
                      QVector<QVector<T> >& out)
{
    assert(in.size() > 0);
    int size = in.size();

    int s = sqrt(size);
    assert((s * s) == size);

    if(out.size() == 0)
    {
        out.resize(s);

        for (int i = 0; i < s; ++i)
            out[i].resize(s);
    }

    int idx = 0;

    for(int i = 0; i < s; ++i)
        for(int j = 0; j < s; ++j, ++idx)
            out[i][j] = in[idx];
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

void Array2QVector(const stir::Array<2, float> & input,
                   QVector<QVector<double> > & output)
{

    if (output.size() == 0)
    {
        output.resize(input.size());

        for (int i = 0; i < input.size(); ++i)
            output[i].resize(input[i].size());
    }

    assert(input.size() == output.size());

    int ii = 0, ij = 0;
    for( int i = input.get_min_index(); i <= input.get_max_index(); ++i, ++ii)
    {
        for (int j = input[i].get_min_index(); j <= input[i].get_max_index(); ++j, ++ij)
        {
            output[ii][ij] = static_cast<double>(input[i][j]);
        }
        ij = 0;
    }
}

}

#endif
