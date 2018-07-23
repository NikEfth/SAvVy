#ifndef SAVVY_H
#define SAVVY_H

#include <QVector>
#include <QProgressDialog>

#include "stir/common.h"
#include "stir/Array.h"
#include "stir/VoxelsOnCartesianGrid.h"

namespace savvy
{
template<class T>
void copy_QVector(const QVector<T> &in, QVector<T> &out, T &_min, T &_max)
{
    assert(in.size() > 0);

    int row_num = in.size();

    if(out.size() != row_num)
    {
        out.resize(row_num);
    }

    for(int i = 0; i < row_num; ++i)
    {
        if(in[i] != 0.0)
        {
            out[i] = in[i];

            if (in[i] >  _max)
            {
                _max = in[i];
            }
            else
            {
                if(in[i]<  _min)
                {
                    _min = in[i];
                }
            }
        }
    }
}

template<class T>
void serialize_QVector(const QVector<QVector<T>> &in, QVector<T> &out, T &_min, T &_max)
{
    assert(in.size() > 0);
    assert(in[0].size() > 0);

    int row_num = in.size();
    int row_size = in[0].size();

    if(out.size() == 0)
    {
        out.resize(row_num * row_size);
    }

    for(int i = 0; i < row_num; ++i)
    {
        for(int j = 0; j < row_size; ++j)
        {
            if(in[i][j] != 0.0)
            {
                out[(row_size * i) + j] = in[i][j];

                if (in[i][j] > _max)
                {
                    _max = in[i][j];
                }
                else
                {
                    if(in[i][j] < _min)
                    {
                        _min = in[i][j];
                    }
                }
            }
        }
    }
}

template<class T>
void serialize_QVector(const QVector<QVector<T>> &in, QVector<QVector<T>> &out, QVector<T> &_min, QVector<T> &_max)
{
    assert(in.size() > 0);
    assert(in[0].size() > 0);

    int row_num  = in.size();
    int row_size = in[0].size();

    if(out.size() == 0)
    {
        out.resize(row_num * row_size);
    }

    for(int i = 0; i < row_num; ++i)
    {
        for(int j =  0; j <  row_size; ++j)
        {
            if (in[i][j] != 0.0)
            {
                out[0][(row_size * i) + j] = in[i][j];

                if (in[i][j] > _max[0])
                {
                    _max[0] = in[i][j];
                }
                else
                {
                    if(in[i][j]< _min[0])
                    {
                        _min[0] = in[i][j];
                    }
                }
            }
        }
    }
}

template<class T>
void serialize_QVector(const QVector<QVector<QVector<T>>> &in, QVector<T> &out, T &_min, T &_max)
{
    assert(in.size() > 0);
    assert(in[0].size() > 0);
    assert(in[0][0].size() > 0);

    int data_num  = in.size();
    int row_num  = in[0].size();
    int row_size = in[0][0].size();

    if(out.size() == 0)
    {
        out.resize(row_num * row_size * data_num);
    }

    for(int i =  0; i <  data_num; ++i)
    {
        for(int j = 0; j < row_num; ++j)
        {
            for(int k = 0; k < row_size; ++k)
            {
                if (in[i][j][k] != 0.0)
                {
                    out[(row_size * j) + k] = in[i][j][k];

                    if (in[i][j][k] >_max)
                    {
                        _max = in[i][j][k];
                    }
                    else
                    {
                        if(in[i][j][k] < _min)
                        {
                            _min = in[i][j][k];
                        }
                    }
                }
            }
        }
    }
}

template<class T>
void serialize_QVector(const QVector<QVector<QVector<T>>> &in, QVector<QVector<T>> &out, QVector<T> &_min, QVector<T> &_max)
{
    assert(in.size() > 0);
    assert(in[0].size() > 0);
    assert(in[0][0].size() > 0);

    assert(out.size() == in.size());

    int data_num  = in.size();
    int row_num  = in[0].size();
    int row_size = in[0][0].size();

    for(int i = 0; i < data_num; ++i)
    {
        for(int j = 0; j < row_num; ++j)
        {
            for(int k = 0; k < row_size; ++k)
            {
                if (in[i][j][k] != 0.0)
                {
                    out[i][(row_size * j) + k] = in[i][j][k];

                    if (in[i][j][k] >_max[i])
                    {
                        _max[i] = in[i][j][k];
                    }
                    else
                    {
                        if(in[i][j][k] < _min[i])
                        {
                            _min[i] = in[i][j][k];
                        }
                    }
                }
            }
        }
    }
}

//!
//! \brief deserialize_QVector
//! \param in
//! \param out
//! \warning Be carefull that [X * Y] = size_row_vector
//!
template<class T>
void deserialize_QVector(const QVector<T> &in, QVector<QVector<T>> &out)
{
    assert(in.size() > 0);

    int size = in.size();
    int size_sqrt = static_cast<int>(sqrt(size));

    if(out.size() == 0)
    {
        out.resize(size_sqrt);

        for(int i = 0; i < size_sqrt; ++i)
        {
            out[i].resize(size_sqrt);
        }
    }

    for(int i = 0; i < size_sqrt; ++i)
    {
        for(int j = 0; j < size_sqrt; ++j)
        {
            out[i][j] = in[(size_sqrt * i) + j];
        }
    }
}

static void decompose_Array1D_to_QVectors_XY(const stir::Array<1, float> &input, QVector<double> &x_values, QVector<double> &y_values)
{
    if (input.size() > static_cast<size_t>(x_values.size()))
    {
        x_values.resize(static_cast<int>(input.size()));
    }

    if(input.size() > static_cast<size_t>(y_values.size()))
    {
        y_values.resize(static_cast<int>(input.size()));
    }

    double x_spacing = static_cast<double>(input.get_max_index() - input.get_min_index()) / static_cast<double>(input.size());

    for(int i = 0; i < input.size(); ++i)
    {
        x_values[i] = static_cast<double>(input.get_min_index()) + (x_spacing * static_cast<double>(i));
        y_values[i] = static_cast<double>(input[input.get_min_index() + i]);
    }
}


static void Array1D_QVector1D(const stir::Array<1, float> &input, QVector<double> &output, double &_min, double &_max)
{
    assert(output.size() > 0);

    for(int i = 0; i < input.size(); ++i)
    {
        if(static_cast<double>(input[input.get_min_index() + i]) != 0.0)
        {
            output[i] = static_cast<double>(input[input.get_min_index() + i]);

            if (static_cast<double>(input[input.get_min_index() + i]) > _max)
            {
                _max = static_cast<double>(input[input.get_min_index() + i]);
            }
            else
            {
                if(static_cast<double>(input[input.get_min_index() + i])<  _min)
                {
                    _min = static_cast<double>(input[input.get_min_index() + i]) ;
                }
            }
        }
    }
}

static void Array2D_QVector1D(const stir::Array<2, float> &input, QVector<double> &output, double &_min, double &_max)
{
    assert(output.size() > 0);

    for(int i = 0; i < input.size(); ++i)
    {
        for(int j = 0; j < input[input.get_min_index() + i].size(); ++j)
        {
            if(static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]) != 0.0)
            {
                output[(input[input.get_min_index() + i].size() * i) + j] = static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]);

                if (static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]) > _max)
                {
                    _max = static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]);
                }
                else
                {
                    if(static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]) < _min)
                    {
                        _min = static_cast<double>(input[input.get_min_index() + i][input[input.get_min_index() + i].get_min_index() + j]);
                    }
                }
            }
        }
    }
}

//!\todo montage
static void Array3D_QVector1D(const stir::Array<3, float> &input, QVector<double> &output, double &_min, double &_max)
{
    assert(output.size() > 0 );
    QVector<double>::iterator ot = output.begin();

    for (stir::Array<3, float>::const_full_iterator it = input.begin_all_const(); it != input.end_all(); ++it, ++ot)
    {
        double val =static_cast<double>(*it);

        if(val != 0.0)
        {
            *ot = val;
            if (val >  _max )
                _max = val ;
            else if(val<  _min)
                _min = val ;
        }
    }
}

//!
//! \brief Array3D_QVector2Ds
//! \param input
//! \param output
//! \param _min
//! \param _max
//! Transform a 3D Vector to a Vector 2D for dipslay
static void Array3D_QVector2Ds(const stir::Array<3, float> & input,
                               QVector<QVector<double> > & output, QVector<double>& _min, QVector<double>& _max)
{

    assert(output.size() > 0 );

    for (int i = input.get_min_index(), idp = 0; i <= input.get_max_index() ; ++i, ++idp)
        for (int j = (input)[i].get_min_index(), idx = 0; j <= (input)[i].get_max_index() ; ++j)
            for (int k = (input)[i][j].get_min_index(); k <= (input)[i][j].get_max_index() ; ++k, ++idx)
            {
                double val =static_cast<double>(input[i][j][k]);

                if(val != 0.0)
                {
                    output[idp][idx] = val;
                    if (val >  _max[idp] )
                        _max[idp] = val ;
                    else if(val<  _min[idp])
                        _min[idp] = val ;
                }
            }
}

static void Array2D_QVector2D(const stir::Array<2, float> & input,
                              QVector<QVector<double> > & output)
{

    if (output.size() == 0)
    {
        output.resize(static_cast<int>(input.size()));

        for (int i = 0; i < input.size(); ++i)
            output[i].resize(static_cast<int>(input[0].size()));
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

static void Array3D_QVector3D(const stir::Array<3, float> & input,
                              QVector<QVector<QVector<double> > > & output)
{
    if (output.size() == 0)
    {
        output.resize(static_cast<int>(input.size()));

        for (int i = 0; i < input.size(); ++i)
        {
            output[i].resize(static_cast<int>(input[0].size()));

            for (int j = 0; j < input[0][0].size(); ++j)
                output[i][j].resize(static_cast<int>(input[0][0].size()));
        }
    }

    assert(input.size() == output.size());

    int ii = 0, ij = 0, ik = 0;
    for( int i = input.get_min_index(); i <= input.get_max_index(); ++i, ++ii)
    {
        for (int j = input[i].get_min_index(); j <= input[i].get_max_index(); ++j, ++ij)
        {
            for( int k = input[i][j].get_min_index(); k <= input[i][j].get_max_index(); ++k, ++ik)
            {
                output[ii][ij][ik] = static_cast<double>(input[i][j][k]);
            }
            ik = 0;
        }
        ij = 0;
    }
}
}

#endif //SAVVY_H
