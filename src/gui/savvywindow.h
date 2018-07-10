#ifndef SAVVYWINDOW_H
#define SAVVYWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMdiSubWindow>

// GUI elements
#include "src/gui/workspace.h"
#include "src/gui/panel_displayed_files.h"
#include "src/gui/panel_opened_file_controls.h"
#include "src/display/display_container.h"
#include "src/display/display_manager.h"

namespace Ui {
class SavvyWindow;
}

//namespace savvy {
//class SavvyWindow;
//}

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

public slots:
    //! Call this function when you want to focus to another window
    void focus_sub_window(QString);

    void display_array(std::shared_ptr<stir::ArrayInterface> _array, QString _name);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();

    void on_actionStart_GUI_tests_triggered();

    //! Update the state of the interface after every subwindown action
    void updateGUI(QMdiSubWindow *activeSubWindow = NULL);

    /** \addtogroup MDI
     *  @{
     */
    //! Append a new window in the ui->mdiArea
    bool append_to_mdi(Display_container *child,
                             bool prepend_to_recent=false,
                             bool minimized = false);

    bool append_to_mdi(Display_manager *child,
                             bool prepend_to_recent=false,
                             bool minimized = false);

    void remove_from_mdi();
    /** @}*/

    /** \addtogroup GUI
     *  @{
     */
    void on_actionDefault_Settings_triggered();

    void on_actionAbout_triggered();

    void on_actionRename_triggered();
    /** @}*/
    void on_actionDuplicate_triggered();

private:

    /** \addtogroup Operations
     *  @{
     */
    bool open_file(const QString &fileName);
    /** @}*/

    /** \addtogroup MDI
     *  @{
     */
    //! Find and \return a pointer to the QMdiSubWindow with the _id
    QMdiSubWindow *findMdiChild(const QString &_id) const;
    //! Create a new Display_container with num_dims the number of dimensions
    Display_container *createDisplayContainer(int num_dims = 1);
    //! Create a new Display_Manager
    Display_manager *createDisplayManager(int num_dims = 3);

    /** @}*/

    //! Hold the path of the last opened file
    QString initial_open_path;

    //! \details This member holds an incremental number for  every window that
    //! opens in the life time of the current run. It can only go upwards, starting from
    //! 1.
    quint16 next_window_id;
    //! The QMdiSubWindow which was previously active.
    DisplayInterface* previous_active;

    /** \addtogroup GUI
     *  @{
     */
    enum { MaxRecentFiles = 10 };

    Ui::SavvyWindow *ui;

    void create_interface();

    void create_docks();

    void create_actions();

    void updateRecentFileActions();

    static bool hasRecentFiles();

    void prependToRecentFiles(const QString &fileName);

    void setRecentFilesVisible(bool visible);

    void openRecentFile();

    void tileSubWindowsVertically();

    void tileSubWindowsHorizontally();

    void shadeSubWindows();
    //! QDockWidget witch will hold the ToolManager
    QDockWidget* dc_tool_manager;
    //! QDockWidget for the Panel_opened_files
    QDockWidget* dc_opened_files;

    QDockWidget* dc_displayed_files;
    //! QDockWidget for Panel_opened_file_controls
    QDockWidget* dc_opened_file_controls;

    QDockWidget* dc_contrast;

    Panel_displayed_files* pnl_displayed_files;

    Workspace* pnl_workspace;

    Panel_opened_file_controls* pnl_opened_file_controls;

    QToolBar *toolBar;

    QAction *recentFileActs[MaxRecentFiles];

    QAction *recentFileSeparator;

    QAction *recentFileSubMenuAct;

    QAction *tileAct;

    QAction *cascadeAct;

    QAction *shadeAct;

    QAction *closeAllAct;

    QAction *tileVerticalAct;

    QAction *tileHorizontalAct;
    /** @}*/

    /** \addtogroup Testing
     *  @{
     */
    //! Ask a question to the user
    int ask(QString);
    //! This function will create a sinc plot. The plot will have 121 points with
    //! offset -60. The x axis is on indeces.
    //! - This test function will write the test array in Workspace
    bool test_display_1d_data();
    //! Similar to test_display_1d_data() but the array has 121 points, with
    //! sampling distance 0.5 (mm) therefore the boundaties are [-30,30).
    //! The x axis is float representing a physical sampling.
    //! - The array data are drawn from Workspace.
    bool test_display_1d_data_physical();
    //! Creates a 2D sinc plot in a matrix [200, 200], from (-100, -100) to (99, 99).
    //! This is the simplest case of 2D plotting handled by the application.
    //! - In this case a stir::Array<2> is created and the values are passed in a
    //! 1D QVector, from savvy::Array2QVector(const stir::Array<2, float> & input,
    //! QVector<QVector<double> > & output) and the 2D matrix is input to the
    //! Display_container_2d.
    //! - The result should look like a sereis of concetric circles with gradially
    //! reduced intensity. The circles should be centered around point (0,0).
    //! - The serialisation is done in the Display_container_2d. This approch should have
    //! a small speed advnatage as, in the same time the search for mininum and
    //! maximum value is performed.
    bool test_display_2d_data();
    //! This test function is similar to the test_display_2d_data(). The same
    //! simulation function, is used. The differences are
    //! # zero offset is used, therefore point (0,0) should be at the lower bottem
    //! corner
    //! # The savvy::serialize_QVector is called before the display function, therefore
    //! the 1D array is the input to Display_container_2d::set_display().
    bool test_display_2d_data_alt();
    //! A test for displaying non square matrices. This test is similar to
    //! test_display_2d_data_alt() but the size of the x axis is smaller than the size of
    //! y axis. The dipslayed gradient should move from left to right.
    //! The serialisation is performed calling the savvy::serialize_QVector()
    bool test_display_2d_data_alt_not_square();
    //! Similar to test_display_2d_data() but the physical sampling is every 0.5 (mm)
    //! therefore the size of the image boundaries are (-50, -50) to (49,49) mm . The
    //! serialisation is performed by calling savvy::serialize_QVector()
    bool test_display_2d_data_physical();
    //! Tests the display with physical sizes and non square form. In this case the
    //! Y axis is shorter and the gradient should move from bottom to top.
    bool test_display_2d_data_physical_not_square();

    bool test_display_3d_data();

    bool test_display_3d_data_alt();

    /** @}*/
};

#endif // SAVVYWINDOW_H
