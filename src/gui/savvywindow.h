#ifndef SAVVYWINDOW_H
#define SAVVYWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QDir>
#include <QActionGroup>

#include <memory>

// GUI elements
#include "src/gui/workspace.h"
#include "src/gui/panel_displayed_files.h"
#include "src/gui/panel_opened_file_controls.h"
#include "src/display/display_container.h"
#include "src/display/display_manager.h"
#include "src/include/external_interface.h"

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
    explicit SavvyWindow(QWidget *parent = nullptr);
    ~SavvyWindow() override;

public slots:
    //! Call this function when you want to focus to another window
    void focus_sub_window(QString) const;

    void display_array(std::shared_ptr<stir::ArrayInterface> _array, QString _name);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionOpen_triggered();

    void on_actionStart_GUI_tests_triggered();

    //! Update the state of the interface after every subwindown action
    void updateGUI(QMdiSubWindow *activeSubWindow = nullptr);

    /** \addtogroup MDI
     *  @{
     */
    //! Append a new window in the ui->mdiArea
    bool append_to_mdi(Display_container *child,
                       bool prepend_to_recent=false,
                       bool minimized = false);

    bool append_to_mdi(Display_manager *child,
                       bool prepend_to_recent=false,
                       bool minimized = false) const;

    void remove_from_mdi();

    void on_actionDuplicate_triggered();

    void on_set_colormap(int _cm);
    /** @}*/

    /** \addtogroup GUI
     *  @{
     */
    void on_actionDefault_Settings_triggered();

    void on_actionAbout_triggered();

    void on_actionRename_triggered();
    /** @}*/

    void on_actionAbout_Plugins_triggered();

private:

    /** \addtogroup Operations
     *  @{
     */
    bool open_file(const QString &fileName, bool _mute_open = false);
    /** @}*/

    /** \addtogroup MDI
     *  @{
     */
    //! Find and \return a pointer to the QMdiSubWindow with the _id
    QMdiSubWindow *findMdiChild(const QString &_id) const;
    //! Create a new Display_Manager
    Display_manager *createDisplayManager(int num_dims = 3);

    bool auto_plot_opened_files;
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

    void prependToRecentFiles(const QString &fileName) const;

    void setRecentFilesVisible(bool visible) const;

    void openRecentFile();

    void tileSubWindowsVertically() const;

    void tileSubWindowsHorizontally() const;

    void shadeSubWindows() const;
    //! QDockWidget witch will hold the ToolManager
    QDockWidget* dc_tool_manager;
    //! QDockWidget for the Panel_opened_files
    QDockWidget* dc_opened_files;

    QDockWidget* dc_displayed_files;
    //! QDockWidget for Panel_opened_file_controls
    QDockWidget* dc_opened_file_controls;

    QDockWidget* dc_contrast;

    Panel_displayed_files* pnl_displayed_files;

    std::shared_ptr<Workspace> pnl_workspace;

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
    QDir pluginsDir;

    QStringList pluginFileNames;

    void loadPlugins();

    void populateMenus(QObject *plugin);

    void addToMenu(ExternalInterface *plugin, const QString text,
                   QMenu *menu);

    QVector<QAction *> loaded_plugins;
    /** @}*/

    /** \addtogroup Testing
     *  @{
     */
    //! Ask a question to the user
    int ask(QString);
    //! This function will create a sinc function and store it in Workspace.
    //! The plot will have 121 points with offset -60. The x axis is on indeces.
    //! The name of the dataset is "test1".
    bool create_test_1d_data();
    //! Creates a diagonal line segment from (-60, 0) to (60, 120)
    //! The name of the dataset is "test1l".
    bool create_test_1d_data_lin();
    //! Create a stir::Array<2, float> and store it in Workspace
    //! The name of the dataset is "test2".
    bool create_test_2d_data();
    //! Creates a stir::Array<2, float> with a saw function and store it in Workspace.
    //! The name of the dataset is "test2l".
    bool create_test_2d_data_lin();
    //! Create a stir::Array<3, float> and store it in Workspace
    //! The name of the dataset is "test3"
    bool create_test_3d_data();
    //! The name of the dataset it "test3l"
    bool create_test_3d_data_lin();

    //! Test the display of a stir::Array<1, float> in a 1D Display_container_1d
    bool test_display_array_1d();

    bool test_display_array_1d_points();
    //! Test the display of a stir::Array<2, float> in a 1D Display_container_1d
    bool test_display_array_2d_in_1d_container();
    //! Test the display of a stir::Array<3, float> in a 1D Display_container_1d
    bool test_display_array_3d_in_1d_container();

    bool test_display_array_2d();

    bool test_display_array_1d_in_2d_container();

    bool test_display_array_3d_in_2d_container();

    bool test_display_array_3d();

    /** @}*/
};

#endif // SAVVYWINDOW_H
