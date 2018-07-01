#ifndef SAVVYWINDOW_H
#define SAVVYWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMdiSubWindow>

#include "src/include/savvy.h"

// GUI elements
#include "src/gui/panel_opened_files.h"
#include "src/gui/panel_opened_file_controls.h"
#include "src/display/display_container.h"

namespace Ui {
class SavvyWindow;
}

namespace savvy {
class SavvyWindow;
}

//!
//! \brief The SavvyWindow class
//! The main window class.
//!
class SavvyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SavvyWindow(QWidget *parent = 0);
    ~SavvyWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();

    void on_actionStart_GUI_tests_triggered();

    //! Update the state of the interface after every subwindown action
    void updateGUI(QMdiSubWindow *activeSubWindow = NULL);

private:
    /** \addtogroup Members for creating and handling the GUI
     *  @{
     */
    Ui::SavvyWindow *ui;

    void create_interface();

    void create_docks();
    //! QDockWidget witch will hold the ToolManager
    QDockWidget* dc_tool_manager;
    //! QDockWidget for the Panel_opened_files
    QDockWidget* dc_opened_files;
    //! QDockWidget for Panel_opened_file_controls
    QDockWidget* dc_opened_file_controls;

    QDockWidget* dc_contrast;

    Panel_opened_files* pnl_opened_files;

    Panel_opened_file_controls* pnl_opened_file_controls;
    /** @}*/

    /** \addtogroup Members for creating and handling QMDI windows
     *  @{
     */

    //!
    //! \brief createMdiChild
    //! \param num_dims The number of dimentions that will be displayed
    //! \return
    //!
    QWidget *createMdiChild(int num_dims = 1);

    //! Append a new window in the ui->mdiArea
    bool append_to_workspace(QWidget *child,
                             bool prepend_to_recent=true,
                             bool minimized = false);

    /** @}*/

    //! Hold the path of the last opened file
    QString initial_open_path;

    //! \details This member holds an incremental number for  every window that
    //! opens in the life time of the current run. It can only go upwards, starting from
    //! 1.
    quint16 next_window_id;



    /** \addtogroup Testing functions
     *  @{
     */
    //! Ask a question to the user
    int ask(QString);
    //! Test display of 1D data
    bool test_display_1d_data();

    /** @}*/
};

#endif // SAVVYWINDOW_H
