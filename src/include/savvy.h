#ifndef SAVVY_H
#define SAVVY_H

#include <QVector>
#include <QProgressDialog>

#include "stir/common.h"
#include "stir/Array.h"
#include "stir/VoxelsOnCartesianGrid.h"

namespace savvy {

template<class T>
void copy_QVector(const QVector<T >& in,
                  QVector<T>& out, T& _min, T& _max)
{
    int row_num = in.size();
    assert(row_num > 0);

    if(out.size() != row_num)
        out.resize(row_num);

    for(int i = 0; i < row_num; ++i)
    {
        T val = in[i];
        if(val != 0.0)
        {
            out[i] = val;

            if (val >  _max)
                _max = val ;
            else if(val<  _min)
                _min = val ;
        }
    }
}


template<class T>
void serialize_QVector(const QVector<QVector<T> >& in,
                       QVector<T>& out, T& _min, T& _max)
{
    assert(in.size() > 0);
    int row_num = in.size();
    assert(in[0].size() > 0);
    int row_size = in[0].size();

    if(out.size() == 0)
        out.resize(row_num*row_size);

    int idx = 0;
    for(int i = 0; i < row_num; ++i)
        for(int j = 0; j < row_size; ++j, ++idx)
        {
            T val = in[i][j];
            if(val != 0.0)
            {
                out[idx] = val;

                if (val >  _max )
                    _max = val ;
                else if(val<  _min)
                    _min = val ;
            }
        }
}

template<class T>
void serialize_QVector(const QVector<QVector<T> > & in,
                       QVector< QVector<T> >& out, QVector<T>& _min, QVector<T>& _max)
{

    assert(in.size() > 0);
    int row_num  = in.size();
    assert(in[0].size() > 0);
    int row_size = in[0].size();

    if(out.size() == 0)
        out.resize(row_num*row_size);

    int  idx = 0;

    for(int j = 0; j < row_num; ++j)
        for(int k =  0; k <  row_size; ++k, ++idx)
        {
            double val =  in[j][k];
            if (val != 0.f)
            {
                out[0][idx] = val;

                if (val >  _max[0] )
                    _max[0] = val ;
                else if(val<  _min[0])
                    _min[0] = val ;
            }
        }
}


template<class T>
void serialize_QVector(const QVector<QVector<QVector<T> > >& in,
                       QVector< QVector<T> >& out, QVector<T>& _min, QVector<T>& _max)
{
    assert(in.size() > 0);
    int data_num  = in.size();
    assert(in[0].size() > 0);
    int row_num  = in[0].size();
    assert(in[0][0].size() > 0);
    int row_size = in[0][0].size();

    assert(out.size() == data_num);

    int idp = 0, idx = 0;
    for(int i =  0; i <  data_num; ++i, ++idp)
    {
        for(int j = 0; j < row_num; ++j)
            for(int k =  0; k <  row_size; ++k, ++idx)
            {
                double val =  in[i][j][k];
                if (val != 0.f)
                {
                    out[idp][idx] = val;

                    if (val >_max[idp])
                        _max[idp] = val;
                    else if(val < _min[idp])
                        _min[idp] = val;
                }
            }
        idx = 0;
    }
}

template<class T>
void serialize_QVector(const QVector<QVector<QVector<T> > >& in,
                       QVector<T> & out, T& _min, T& _max)
{
    assert(in.size() > 0);
    int data_num  = in.size();
    assert(in[0].size() > 0);
    int row_num  = in[0].size();
    assert(in[0][0].size() > 0);
    int row_size = in[0][0].size();

    if(out.size() == 0)
        out.resize(row_num*row_size*data_num);

    int idx = 0;
    for(int i =  0; i <  data_num; ++i)
    {
        for(int j = 0; j < row_num; ++j)
            for(int k =  0; k <  row_size; ++k, ++idx)
            {
                double val =  in[i][j][k];
                if (val != 0.f)
                {
                    out[idx] = val;

                    if (val >_max)
                        _max = val;
                    else if(val < _min)
                        _min = val;
                }
            }
        idx = 0;
    }
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

static void decompose_Array1D_to_QVectors_XY(const stir::Array<1, float> & input,
                                             QVector<double> & x_values, QVector<double> & y_values)
{

    if (input.size() > x_values.size())
        x_values.resize(static_cast<int>(input.size()));

    if(input.size() > y_values.size())
        y_values.resize(static_cast<int>(input.size()));

    QVector<double>::iterator xot = x_values.begin();
    QVector<double>::iterator yot = y_values.begin();

    double x_spacing = static_cast<double>(input.get_max_index() - input.get_min_index()) / input.get_length();
        int idx = 0;
    for (stir::Array<1, float>::const_full_iterator it = input.begin_all_const(); it != input.end_all(); ++it, ++xot, ++yot,
         ++idx)
    {
        *yot = static_cast<double>(*it);
        *xot = static_cast<double>(input.get_min_index()) + x_spacing*idx;
    }
}


static void Array1D_QVector1D(const stir::Array<1, float> & input,
                              QVector<double> & output, double& _min, double& _max)
{

    assert(output.size() > 0 );
    QVector<double>::iterator ot = output.begin();

    for (stir::Array<1, float>::const_full_iterator it = input.begin_all_const(); it != input.end_all(); ++it, ++ot)
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

static void Array2D_QVector1D(const stir::Array<2, float> & input,
                              QVector<double> & output, double& _min, double& _max)
{

    assert(output.size() > 0 );
    QVector<double>::iterator ot = output.begin();

    for (stir::Array<2, float>::const_full_iterator it = input.begin_all_const(); it != input.end_all(); ++it, ++ot)
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

//!\todo montage
static void Array3D_QVector1D(const stir::Array<3, float> & input,
                              QVector<double> & output, double& _min, double& _max)
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

#endif
