#ifndef SAVVYWINDOW_H
#define SAVVYWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMdiSubWindow>

// GUI elements
#include "src/gui/panel_opened_files.h"
#include "src/gui/panel_opened_file_controls.h"
#include "src/display/display_container.h"

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

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();

    void on_actionStart_GUI_tests_triggered();

    //! Update the state of the interface after every subwindown action
    void updateGUI(QMdiSubWindow *activeSubWindow = NULL);

    /** \addtogroup Members for creating and handling QMDI windows
     *  @{
     */
    //! Append a new window in the ui->mdiArea
    bool append_to_workspace(Display_container *child,
                             bool prepend_to_recent=true,
                             bool minimized = false);

    void remove_from_workspace();
    /** @}*/

private:

    /** \addtogroup File Operations
     *  @{
     */

    bool open_file(const QString &fileName);

    /** @}*/

    /** \addtogroup Members for creating and handling QMDI windows
     *  @{
     */
    //! Find and \return a pointer to the QMdiSubWindow with the _id
    QMdiSubWindow *findMdiChild(const QString &_id) const;
    //! Create a new Display_container with num_dims the number of dimensions
    Display_container *createMdiChild(int num_dims = 1);
    /** @}*/

    //! Hold the path of the last opened file
    QString initial_open_path;

    //! \details This member holds an incremental number for  every window that
    //! opens in the life time of the current run. It can only go upwards, starting from
    //! 1.
    quint16 next_window_id;
    //! The QMdiSubWindow which was previously active.
    Display_container* previous_active;

    /** \addtogroup Members for creating and handling the GUI
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
    //! QDockWidget witch will hold the ToolManager
    QDockWidget* dc_tool_manager;
    //! QDockWidget for the Panel_opened_files
    QDockWidget* dc_opened_files;
    //! QDockWidget for Panel_opened_file_controls
    QDockWidget* dc_opened_file_controls;

    QDockWidget* dc_contrast;

    Panel_opened_files* pnl_opened_files;

    Panel_opened_file_controls* pnl_opened_file_controls;

    QToolBar *toolBar;

    QAction *recentFileActs[MaxRecentFiles];

    QAction *recentFileSeparator;

    QAction *recentFileSubMenuAct;

    QAction *tileAct;

    QAction *cascadeAct;

    QAction *closeAllAct;

    QAction *tileVerticalAct;

    QAction *tileHorizontalAct;
    /** @}*/

    /** \addtogroup Testing functions
     *  @{
     */
    //! Ask a question to the user
    int ask(QString);
    //! This function will create a sinc plot. The plot will have 121 points with
    //! offset -60.
    bool test_display_1d_data();
    //! Similar to test_display_1d_data() but the array has 121 points, with
    //! sampling distance 0.5 (mm) therefore the boundaties are [-30,30).
    bool test_display_1d_data_physical();
    //! Creates a 2D sinc plot in a matrix [200, 200], from (-100, -100) to (99, 99)
    bool test_display_2d_data();

    bool test_display_2d_data_alt();
    //! Similar to test_display_2d_data() but the physical sampling is every 0.5 (mm)
    //! therefore the size of the image boundaries are (-50, -50) to (49,49) mm .
    bool test_display_2d_data_physical();

    /** @}*/
};

#endif // SAVVYWINDOW_H
