#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <QWidget>
#include "display_container.h"
#include "common_display.h"

using namespace display;

namespace Ui {
class Display_manager;
}

class Display_manager : public DisplayInterface
{
    Q_OBJECT

public:
    explicit Display_manager(int _my_id, int _num_dim, QWidget *parent = 0);

    explicit Display_manager(int _my_id, stir::ArrayInterface* __display, QWidget *parent = 0);

    ~Display_manager();

    /** \addtogroup Setters
     *  @{
     */
    inline void set_file_name(QString _s)
    {
        fullFileName = _s;
        this->setWindowTitle(_s);
    }

    //!
    virtual void set_display(void*_in)
    {
        //        _display->set_display(_in);
    }

    void set_no_controls(bool _f);

    //! Set the Colormap of choise
    virtual void set_color_map(int i)
    {
        _display->set_color_map(i);
    }
    /** @}*/
    inline Display_container* get_display()
    {
        return _display;
    }

    inline QString get_label(int _page, int num_pages)
    {
        return QString::number(_page) + " / " + QString::number(num_pages);
    }

signals:
    //! Signal to let the application know that this window should be
    //! removed from various places.
    void aboutToClose();

protected:
    //! We hack this QEvent because upon close we emit aboutToClose()
    //! to let know the application that this window must be removed
    //! from various places.
    void closeEvent(QCloseEvent *event) override;

private slots:

    void updated_display(int position);

    void initialised_display();
private:
    Ui::Display_manager *ui;

    Display_container* _display;
};

#endif // DISPLAY_MANAGER_H
