#ifndef STACKPROCESSOR_H
#define STACKPROCESSOR_H

#include <QWidget>
#include "imageprocessor.h"
#include "savvy.h"

namespace Ui {
class StackProcessor;
}

class StackProcessor : public ImageProcessor
{
    Q_OBJECT

public:
    explicit StackProcessor(std::shared_ptr<Workspace> workspace = nullptr,
                            QDialog *parent = nullptr);
    ~StackProcessor() override;

    virtual void apply() override;
private slots:
    void on_pushButton_clicked();

    void on_sld_slice_num_sliderMoved(int position);

    void on_psh_Apply_clicked();
    
    void on_sld_slice_num_valueChanged(int value);

private:

    void split_array();

    void resort();

    Ui::StackProcessor *ui;

    bool no_stack = true;

    bool slice_on_slice = true;

    stir::Array<3, float> *current_data_set_ptr = nullptr;
};

#endif // STACKPROCESSOR_H
