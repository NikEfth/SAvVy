#ifndef STACKPROCESSORUI_H
#define STACKPROCESSORUI_H

#include <QDialog>
#include "savvy.h"
#include "workspace.h"

namespace Ui {
class StackProcessorUI;
}

class StackProcessorUI : public QDialog
{
    Q_OBJECT

public:
    explicit StackProcessorUI(std::shared_ptr<Workspace> workspace = nullptr,
                            QDialog *parent = nullptr);
    ~StackProcessorUI() override;

private slots:

    void on_pushButton_clicked();
    
    void on_sld_slice_num_valueChanged(int value);

    void on_psh_split_on_slide_clicked();

    void on_psh_reverse_order_clicked();

    void on_psh_human_order_clicked();

    void on_psh_concacenate_clicked();

    bool refresh_dataset();

private:

    void split_array();

    Ui::StackProcessorUI *ui;

    bool no_stack = true;

    bool slice_on_slice = true;

    stir::Array<3, float> *current_data_set_ptr = nullptr;

    std::shared_ptr<Workspace> m_workspace_sptr = nullptr;
};

#endif // STACKPROCESSOR_H
