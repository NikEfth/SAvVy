#ifndef STACKPROCESSOR_H
#define STACKPROCESSOR_H

#include <QObject>

#include <memory>

#include <stir/Array.h>

#include "savvy.h"

class StackProcessor : public QObject
{
    Q_OBJECT
public:
    explicit StackProcessor(stir::Array<3, float> *data_ptr,
                            QObject *parent = nullptr);

    virtual bool execute() = 0;

signals:
    void finished();

public slots:

protected:
    stir::Array<3, float>* m_data_ptr;
};

#endif // STACKPROCESSOR_H
