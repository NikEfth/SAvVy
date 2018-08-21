#ifndef CTRL_LEVELS_H
#define CTRL_LEVELS_H

#include "src/gui/watchdog.h"

//#include "src/display_buildblock/screen_manager.h"
#include "src/display/histogram_manager.h"

namespace Ui {
class CTRL_Levels;
}

//!
//! \brief The CTRL_Levels class
//! The base widget for the display of
//! the Contrast and WL operations.
class CTRL_Levels : public WatchDog
{
    Q_OBJECT

public:
    explicit CTRL_Levels(QWidget *parent = nullptr);
    ~CTRL_Levels() override;

    virtual void setContainer(DisplayInterface * _s) override;
    //! Unset the current Screen_manager.
    virtual void unsetContainer() override;

private:
    Ui::CTRL_Levels *ui = nullptr;

public slots:
        void display_updated();

protected:
    //! Pointer to hold the current connected Screen_manager.
    //Screen_manager* sc;
    //! Pointer to hold the BarScreenWL.
    Histogram_manager* my_histogram;

private slots:

    void updateMarkers(const double _min, const double _max);

    void updateWL(const double window, const double lavel);
    //! Restore the original vis_min and max_viz. Usually this means full contrast range.
    void on_btn_reset_clicked();
    //! Optimisation of the contrast range.
    //! Each click of the button narrows the contrast range 5%,
    //! excluding the bin with the valuw zero.
    void on_btn_auto_clicked();
    //! Set het upper threshold.
    void on_le_max_returnPressed();
    //! Set the lower threshold
    void on_le_min_returnPressed();
    //! Set the image Level.
    void on_le_level_returnPressed();
    //! Set the image window.
    void on_le_win_returnPressed();

private:

    double m_min_value;

    double m_max_value;

    double m_window;

    double m_level;
};

#endif // CONTRASTMAN_H
