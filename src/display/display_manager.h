#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <QWidget>
#include "common_display.h"

using namespace display;

namespace Ui {
class Display_manager;
}

class Display_manager : public DisplayInterface
{
    Q_OBJECT

public:
    //! Create Display_manager without any data
    explicit Display_manager(int _my_id, int _num_dim, QWidget *parent = nullptr);
    //! Create Display_manager with data to display from stir::ArrayInterface
    explicit Display_manager(int _my_id, stir::ArrayInterface* __display, QWidget *parent = nullptr);
    //! Create Display_manager with data to display and set the ColorMap
    explicit Display_manager(int _my_id, stir::ArrayInterface* __display,
                             QSharedPointer<QwtLinearColorMap> _def_colormap = nullptr, QWidget *parent = nullptr);
    //! Create Display_manager with data to display from  QVector
    explicit Display_manager(int _my_id,
                             std::shared_ptr<QVector<QVector<QVector<double> > > > _prod_image,
                             QWidget *parent = nullptr);

    ~Display_manager() override;

    /** \addtogroup Setters
     *  @{
     */
    inline void set_file_name(QString _s)
    {
        fullFileName = _s;
        this->setWindowTitle(_s);
    }

    virtual void set_curve(const QVector<double> & _x_array,
                           const QVector<double> & _y_array,
                           bool replace = true, int after = 1,
                           bool symbols = false, bool line = false) override;

    //!
    virtual void set_display(void*_in)
    {
        // to silence warning
        if(_in)
        {

        }

        //        _display->set_display(_in);
    }

    void set_no_controls(bool _f) override;

    //! Set the Colormap of choise
    inline void set_color_map(const QSharedPointer<QwtLinearColorMap> i)
    {
        _display->set_color_map(i);
    }
    /** @}*/

    inline QString get_label(int _page, int num_pages)
    {
        return QString::number(_page) + " / " + QString::number(num_pages);
    }

    virtual std::shared_ptr< QVector<double> >  get_x_values() const override;

    virtual std::shared_ptr< QVector<double> >  get_y_values() const override;

    virtual size_t get_num_points() const override;

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
    void on_psh_save_image_clicked();

private:
    Ui::Display_manager *ui = nullptr;

};

#endif // DISPLAY_MANAGER_H
