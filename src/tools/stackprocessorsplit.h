#ifndef STACKPROCESSORSPLIT_H
#define STACKPROCESSORSPLIT_H

#include "stackprocessor.h"

class StackProcessorSplit : public StackProcessor
{
    Q_OBJECT
public:
    StackProcessorSplit(stir::Array<3, float>  *data_sptr,
                        QObject *parent = nullptr);

    virtual bool execute();

    void set_split_slide(int val);

    void set_recrusive(bool val);

    std::shared_ptr<stir::VoxelsOnCartesianGrid<float> > get_slit_array(int i) const;

private:
    int split_pos = 0;

    bool recrusive = false;

    QVector<std::shared_ptr<stir::VoxelsOnCartesianGrid<float> > > res;
};

#endif // STACKPROCESSORSPLIT_H
