#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QDialog>
#include "../gui/workspace.h"

#include <stir/VoxelsOnCartesianGrid.h>

#include <memory>

class ImageProcessor : public QDialog
{
    Q_OBJECT
public:
    explicit ImageProcessor(std::shared_ptr<Workspace> workspace = nullptr,
                            QDialog *parent = nullptr);

    virtual void apply() = 0;
signals:

public slots:

protected:
    std::shared_ptr<Workspace> m_workspace_sptr = nullptr;
};

#endif // IMAGEPROCESSOR_H
