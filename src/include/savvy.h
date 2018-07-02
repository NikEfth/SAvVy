#ifndef SAVVY_H
#define SAVVY_H

#include <QVector>

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

}

#endif
