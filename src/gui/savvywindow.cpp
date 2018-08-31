#include "src/include/savvy.h"
#include "savvywindow.h"
#include "ui_savvywindow.h"
#include "src/display/display_container_1d.h"
#include "src/display/display_container_2d.h"
#include "src/display/display_container_3d.h"
#include "src/display/display_manager.h"
#include "pluginsdialog.h"
#include "savvy_settings.h"
#include "aboutdialog.h"

#include "stir/is_null_ptr.h"
#include "stir/common.h"
#include "stir/Coordinate2D.h"

#include "stackprocessorUI.h"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QPluginLoader>

USING_NAMESPACE_STIR

SavvyWindow::SavvyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SavvyWindow)
{
    ui->setupUi(this);
    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());

    if(settings.contains("defaultTabbedViewMode"))
    {
        bool state = settings.value("defaultTabbedViewMode").toBool();
        if (state)
            ui->mdiArea->setViewMode( QMdiArea::ViewMode::TabbedView);
    }

    if(settings.contains("AutoPlotOpenedImages"))
        auto_plot_opened_files = settings.value("AutoPlotOpenedImages").toBool();
    else
        auto_plot_opened_files = true;

    next_dataset_id = 0;

    next_display_id = 0;

    create_interface();

    loadPlugins();

    // This connection will update the window on every interaction
    // It is the main "refresh all" interface function
    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            this, &SavvyWindow::updateGUI);
}

SavvyWindow::~SavvyWindow()
{
    for (int i = 0 ; i < loaded_plugins.size(); ++i)
        ui->menuPlugins->removeAction(loaded_plugins[i]);
    delete ui;
}

void SavvyWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if (ui->mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        QSettings settings;
        settings.setValue("mainWindowGeometry", saveGeometry());
        settings.setValue("mainWindowState", saveState());
        event->accept();
    }
}

void SavvyWindow::on_actionDefault_Settings_triggered()
{
    Savvy_settings settings(this);
    settings.exec();
}

/*
 *
 *
 *      TRIGGERS
 *
 */

void SavvyWindow::on_actionOpen_triggered()
{
    const QStringList fileNames =
            QFileDialog::getOpenFileNames(this,
                                          tr("Open File"),
                                          initial_open_path,
                                          tr("All Files (*.hs *.hv *.dicom *.dcm);;"
                                             "STIR Interfile Header (*.hs);;"
                                             "STIR Image Header (*.hv);;"
                                             "Dicom Images (*.dicom);;"
                                             ));

    if (fileNames.size() == 0)
        return;

    bool _mute = false;

    if (fileNames.size() > 1)
        _mute = true;

    for (QString fileName : fileNames)
        open_file(fileName, _mute);
}

bool SavvyWindow::on_actionSave_triggered()
{
    if(pnl_workspace->get_num_of_openned_files() == 0)
    {
        QMessageBox msgBox;
        msgBox.setText("No opened files found");
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Critical);
        return msgBox.exec();
    }

    const QString fileName =
            QFileDialog::getSaveFileName(this,
                                         "Save as",
                                         pnl_workspace->get_current_name(),
                                         tr("STIR Image Header (*.hv);;")
                                            );

    if (fileName.isEmpty())
        return false;

    return pnl_workspace->write_current_file_to_disk(fileName);
}


/*
 *
 *
 *
 * FILE OPERATIONS
 *
 *
 *
 */

bool SavvyWindow::open_file(const QString& fileName, bool _mute_open)
{

    std::shared_ptr <stir::ArrayInterface> tmp_sptr =
            pnl_workspace->open_array(fileName);

    if(is_null_ptr(tmp_sptr))
    {
        QMessageBox msgBox;
        msgBox.setText("Error opening file "+fileName);
        msgBox.setStandardButtons(QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setIcon(QMessageBox::Critical);
        return msgBox.exec();
    }

    prependToRecentFiles(fileName);

    if(auto_plot_opened_files || !_mute_open)
        display_array(tmp_sptr, pnl_workspace->get_current_name());

    return true;
}

/*
 *
 *
 *  MDI - WINDOWS FUNCTIONS
 *
 *
 */

void SavvyWindow::updateGUI(QMdiSubWindow * activeSubWindow)
{
    bool hasMdiChild = (ui->mdiArea->subWindowList().size() != 0);

    pnl_displayed_files->setEnabled(hasMdiChild);
    ui->menuSlice_Annotations->setEnabled(hasMdiChild);
    ui->menuAdjust->setEnabled(hasMdiChild);

    if (! stir::is_null_ptr(activeSubWindow))
    {
        DisplayInterface * active_display_container =
                dynamic_cast<DisplayInterface *>(activeSubWindow->widget());

        if (active_display_container != previous_active)
        {

            pnl_displayed_files->set_active(active_display_container->get_my_id());

            if (!dc_contrast->isHidden())
            {
                ctrl_levels->unsetContainer();
                ctrl_levels->setContainer(active_display_container);
            }

            previous_active = active_display_container;
        }
    }

}

void SavvyWindow::focus_sub_window(QString _id) const
{
    if (QMdiSubWindow *existing = findMdiChild(_id)) {
        ui->mdiArea->setActiveSubWindow(existing);
    }
}

Display_manager *SavvyWindow::createDisplayManager(int num_dims)
{
    Display_manager *ret = new Display_manager(next_dataset_id, num_dims, this);
    next_dataset_id++;
    return ret;
}

QMdiSubWindow *SavvyWindow::findMdiChild(const QString &_id) const
{

    for(QMdiSubWindow *window: ui->mdiArea->subWindowList()) {
        DisplayInterface *mdiChild = qobject_cast<DisplayInterface *>(window->widget());
        if (QString::number(mdiChild->get_my_id()) == _id)
            return window;
    }
    return nullptr;

}

bool SavvyWindow::append_to_mdi(Display_container *child,
                                bool prepend_to_recent,
                                bool minimized)
{
    // to silence warning
    if(child || prepend_to_recent || minimized)
    {

    }

    //    QObject::connect(child, &Display_container::aboutToClose, this, &SavvyWindow::remove_from_mdi);

    //    ui->mdiArea->addSubWindow(child);
    //    ui->mdiArea->setFocusPolicy(Qt::StrongFocus);
    //    pnl_displayed_files->appendToOpenedList(child);

    //    if (!minimized)
    //        child->show();
    //    else
    //        child->showMinimized();

    return true;
}

bool SavvyWindow::append_to_mdi(Display_manager *subWindow,
                                bool prepend_to_recent,
                                bool minimized)
{

    QObject::connect(subWindow, &Display_manager::aboutToClose, this, &SavvyWindow::remove_from_mdi);

    ui->mdiArea->addSubWindow(subWindow);
    pnl_displayed_files->appendToOpenedList(subWindow, next_display_id);
    ++next_display_id;

    if (prepend_to_recent &&
            subWindow->get_file_name().size() > 0)
        prependToRecentFiles(subWindow->get_file_name());

    if (!minimized)
        subWindow->show();
    else
        subWindow->showMinimized();

    subWindow->parentWidget()->setFixedSize(subWindow->sizeHint());

    return true;
}

void SavvyWindow::remove_from_mdi()
{
    // Find who send this
    DisplayInterface* src =
            dynamic_cast<DisplayInterface *>(sender());

    pnl_displayed_files->removeFromOpenedList(src);
    // Disconnect from tool
//    toolMan->unset();
    // Disconnect from contrast window
    ctrl_levels->unsetContainer();
    // The Math Manager must have an up-to-date list of the opened
    // files.
    //        mathMan->updateOpenedFiles();
    // If another similar type of connection exist in the future
    // it has to be disconnected here.
}

void SavvyWindow::display_array(std::shared_ptr<stir::ArrayInterface> _array,
                                QString _name)
{

    int dims = _array->get_num_dimensions();

    switch (dims) {
    case 1:
    {
        Display_manager * disp = new Display_manager(next_dataset_id, _array.get(), this);
        disp->set_file_name(_name);
        ++next_dataset_id;

        if (is_null_ptr(disp))
            return;

        append_to_mdi(disp);
        return;
    }
    case 2:
    {
        Display_manager* disp = new Display_manager(next_dataset_id, _array.get(), this);
        disp->set_file_name(_name);
        ++next_dataset_id;

        if (is_null_ptr(disp))
            return;

        append_to_mdi(disp);
        return;
    }
    case 3:
    {
        Display_manager* disp = new Display_manager(next_dataset_id, _array.get(), this);
        disp->set_file_name(_name);
        ++next_dataset_id;

        if (is_null_ptr(disp))
            return;

        append_to_mdi(disp);

        return;
    }
    }
}


void SavvyWindow::on_actionDuplicate_triggered()
{
    Display_container* active =
            qobject_cast<Display_container *>(ui->mdiArea->activeSubWindow()->widget());

    // to silence warning
    if(active)
    {

    }

    //    active->rename(result);

    //    pnl_opened_files->rename(active->get_my_id(), result);
}

/*
 *
 *
 *      GUI STAFF
 *
 */

void SavvyWindow::create_interface()
{
    QDir init_dir = QDir::homePath();
    initial_open_path = init_dir.absolutePath();
    this->setWindowIcon(QIcon(":resources/icons8-microbeam-radiation-therapy-80.png"));

    create_docks();

    create_actions();
}

void SavvyWindow::create_docks()
{
    dc_contrast = new QDockWidget("Contrast and WL",this);
    ctrl_levels.reset(new CTRL_Levels(this));
    dc_contrast->setWidget(ctrl_levels.get());
    this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea,dc_contrast);
    dc_contrast->setFloating(true);
    dc_contrast->setVisible(false);

    dc_tool_manager = new QDockWidget("Tools", this);
//    toolMan.reset(new ToolManager(this));
//    dc_tool_manager->setWidget(toolMan.get());

    dc_opened_files = new QDockWidget("Opened Files", this);
    pnl_workspace.reset(new Workspace(this));
    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            pnl_workspace.get(), &Workspace::updateGUI);
    connect(pnl_workspace.get(), &Workspace::display_current,
            this, &SavvyWindow::display_array);
    dc_opened_files->setWidget(pnl_workspace.get());

    dc_displayed_files = new QDockWidget("Displayed Files", this);
    pnl_displayed_files = new Panel_displayed_files(this);
    connect(pnl_displayed_files, &Panel_displayed_files::double_clicked_item,
            this, &SavvyWindow::focus_sub_window);
    dc_displayed_files->setWidget(pnl_displayed_files);

    this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dc_contrast);
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dc_tool_manager);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_files);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_displayed_files);
//    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_file_controls);
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }
static inline QString fileKey() { return QStringLiteral("file"); }

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

static QStringList readRecentFiles(QSettings &settings)
{
    QStringList result;
    const int count = settings.beginReadArray(recentFilesKey());
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return result;
}

void SavvyWindow::create_actions()
{
    ui->menuFile->addSeparator();

    QMenu *recentMenu = ui->menuFile->addMenu(tr("Recent..."));
    connect(recentMenu, &QMenu::aboutToShow, this, &SavvyWindow::updateRecentFileActions);
    recentFileSubMenuAct = recentMenu->menuAction();

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = recentMenu->addAction(QString(), this, &SavvyWindow::openRecentFile);
        recentFileActs[i]->setVisible(false);
    }

    recentFileSeparator = ui->menuFile->addSeparator();

    setRecentFilesVisible(SavvyWindow::hasRecentFiles());

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, &QAction::triggered, ui->mdiArea, &QMdiArea::tileSubWindows);
    ui->menuWindow->addAction(tileAct);

    tileVerticalAct = new QAction(QIcon(":/icons/resources/other_icons/htile.png") , tr("Tile Vertically"), this);
    tileVerticalAct->setStatusTip(tr("Tile the windows vertically"));
    connect(tileVerticalAct, &QAction::triggered, this, &SavvyWindow::tileSubWindowsVertically);
    ui->menuWindow->addAction(tileVerticalAct);

    tileHorizontalAct = new QAction(QIcon(":/icons/resources/other_icons/vtile.png"), tr("Tile Horizontally"), this);
    tileHorizontalAct->setStatusTip(tr("Tile the windows horizontally"));
    connect(tileHorizontalAct, &QAction::triggered, this, &SavvyWindow::tileSubWindowsHorizontally);
    ui->menuWindow->addAction(tileHorizontalAct);

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, &QAction::triggered, ui->mdiArea, &QMdiArea::cascadeSubWindows);
    ui->menuWindow->addAction(cascadeAct);

    ui->menuWindow->addSeparator();

    shadeAct = new QAction(tr("&Shade"), this);
    shadeAct->setStatusTip(tr("Shade all windows"));
    connect(shadeAct, &QAction::triggered, this, &SavvyWindow::shadeSubWindows);
    ui->menuWindow->addAction(shadeAct);

    ui->menuWindow->addSeparator();

    closeAllAct = new QAction(QIcon(":/icons/resources/gimp-pro/images/gimp-clsoe.png"),tr("&Close All"), this);
    closeAllAct->setStatusTip(tr("Close all windows"));
    connect(closeAllAct, &QAction::triggered, ui->mdiArea, &QMdiArea::closeAllSubWindows);
    ui->menuWindow->addAction(closeAllAct);
}

void SavvyWindow::tileSubWindowsVertically() const
{
    if (ui->mdiArea->subWindowList().isEmpty())
        return;

    QPoint position(0, 0);

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        QRect rect(0, 0, ui->mdiArea->width(), ui->mdiArea->height() / ui->mdiArea->subWindowList().count());
        window->setGeometry(rect);
        window->move(position);
        position.setY(position.y() + window->height());
    }
}

void SavvyWindow::tileSubWindowsHorizontally() const
{
    if (ui->mdiArea->subWindowList().isEmpty())
        return;

    QPoint position(0, 0);

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        QRect rect(0, 0, ui->mdiArea->width() / ui->mdiArea->subWindowList().count(), ui->mdiArea->height());
        window->setGeometry(rect);
        window->move(position);
        position.setX(position.x() + window->width());
    }
}

void SavvyWindow::shadeSubWindows() const
{
    if (ui->mdiArea->subWindowList().isEmpty())
        return;

    foreach (QMdiSubWindow *window, ui->mdiArea->subWindowList()) {
        window->showShaded();
    }
}

bool SavvyWindow::hasRecentFiles()
{
    QSettings settings;
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

void SavvyWindow::prependToRecentFiles(const QString &fileName) const
{
    QSettings settings;

    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    if (oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentFilesVisible(!recentFiles.isEmpty());
}

void SavvyWindow::setRecentFilesVisible(bool visible) const
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

void SavvyWindow::updateRecentFileActions()
{
    QSettings settings;

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    for (int i = 0; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for (int i = 0; i < MaxRecentFiles; ++i)
        recentFileActs[i]->setVisible(false);
}

void SavvyWindow::openRecentFile()
{
    if (const QAction *action = qobject_cast<const QAction *>(sender()))
        open_file(action->data().toString());
}

void SavvyWindow::on_actionRename_triggered()
{

    // Old rename which used to rename the display containers
    //    QString result = QInputDialog::getText(0, "Rename", "New name:");
    //    if (pnl_workspace->has_grouped_items())
    //    {
    //        QVector<QString> groupped = pnl_workspace->get_groupped();
    //        int inc = 1;
    //        for (QString i : groupped)
    //        {
    //            QMdiSubWindow* a = findMdiChild(i);
    //            if (! stir::is_null_ptr(a))
    //            {
    //                Display_container*  s = dynamic_cast<Display_container* >(a->widget());
    //                if (! stir::is_null_ptr(s))
    //                {
    //                    QString tmp = result + "_" + QString::number(inc);
    //                    s->rename(tmp);
    //                    //                    s->setWindowTitle(tmp);
    //                    pnl_workspace->rename(i, tmp);
    //                    ++inc;
    //                }
    //            }
    //        }
    //    }
    //    else
    //    {
    //        Display_container* active =
    //                qobject_cast<Display_container *>(ui->mdiArea->activeSubWindow()->widget());
    //        active->rename(result);
    //        //        active->setWindowTitle(result);
    //        pnl_workspace->rename(active->get_my_id(), result);
    //    }
}

/*
 *
 *
 * TESTS
 *
 *
 */

void SavvyWindow::on_actionStart_GUI_tests_triggered()
{
    bool all_tests = true;

    all_tests = create_test_1d_data();

    all_tests = create_test_1d_data_lin();

    all_tests = create_test_2d_data();

    all_tests = create_test_2d_data_lin();

    all_tests = create_test_3d_data();

    all_tests = create_test_3d_data_lin();


    all_tests = test_display_array_1d();

    all_tests = test_display_array_1d_points();

    all_tests = test_display_array_2d_in_1d_container();

    all_tests = test_display_array_3d_in_1d_container();


    all_tests = test_display_array_2d();

    all_tests = test_display_array_1d_in_2d_container();

    all_tests = test_display_array_3d_in_2d_container();


    all_tests = test_display_array_3d();

}

int SavvyWindow::ask(QString question)
{
    QMessageBox msgBox;
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}


bool SavvyWindow::create_test_1d_data()
{

    const IndexRange<1> range(-60,60);
    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->create_array_ptr<1>(range, "test1");
    stir::Array<1, float>* test1 = dynamic_cast<stir::Array<1, float>* >(test1_sptr.get());

    for (int i = test1->get_min_index(); i <= test1->get_max_index() ; ++i)
    {
        if(i != 0)
        {
            float f = static_cast<float>(i);
            test1->at(i) = sin(f) / f;
        }
        else
            test1->at(i) = 1;
    }

    return true;
}

bool SavvyWindow::create_test_1d_data_lin()
{

    const IndexRange<1> range(-60,60);
    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->create_array_ptr<1>(range, "test1l");
    stir::Array<1, float>* test1 = dynamic_cast<stir::Array<1, float>* >(test1_sptr.get());

    float x = 0.f;
    for (int i = test1->get_min_index(); i <= test1->get_max_index() ; ++i, x += 1.f)
    {
        test1->at(i) = x;
    }

    return true;
}

bool SavvyWindow::create_test_2d_data()
{

    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->create_array_ptr<2>(range, "test2");
    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

    for (int i = test2->get_min_index(); i <= test2->get_max_index() ; ++i)
        for (int j = (*test2)[i].get_min_index(); j <= (*test2)[i].get_max_index() ; ++j)
        {
            float f = static_cast<float>(sqrt(i*i + j*j));
            if( f != 0.0f)
                (*test2)[i][j]  = sin(f) / f;
            else
                (*test2)[i][j] = 1;
        }

    return true;
}

bool SavvyWindow::create_test_2d_data_lin()
{

    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->create_array_ptr<2>(range, "test2l");
    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

    float x  = 0.f;
    float y  = 1.f;
    for (int i = test2->get_min_index(); i <= test2->get_max_index() ; ++i, y +=1.f)
    {
        for (int j = (*test2)[i].get_min_index(); j <= (*test2)[i].get_max_index() ; ++j, x += 1.f)
        {
            (*test2)[i][j]  = x*y;
        }
        x = 0.f;
    }

    return true;
}

bool SavvyWindow::create_test_3d_data()
{

    const IndexRange<3> range(stir::Coordinate3D<int>(-30,-60, -60),stir::Coordinate3D<int>(29,59, 59));
    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->create_array_ptr<3>(range, "test3");
    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

    for (int i = test5->get_min_index(); i <= test5->get_max_index() ; ++i)
        for (int j = (*test5)[i].get_min_index(); j <= (*test5)[i].get_max_index() ; ++j)
            for (int k = (*test5)[i][j].get_min_index(); k <= (*test5)[i][j].get_max_index() ; ++k)
            {
                float f = static_cast<float>(sqrt(i*i + k*k + j*j));
                if( f != 0.0f)
                    (*test5)[i][j][k] = sin(f) / f;
                else
                    (*test5)[i][j][k] = 1;
            }

    return true;
}

bool SavvyWindow::create_test_3d_data_lin()
{

    const IndexRange<3> range(stir::Coordinate3D<int>(-30,-60, -60),stir::Coordinate3D<int>(29,59, 59));
    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->create_array_ptr<3>(range, "test3l");
    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

    float x  = 0.f;
    float y  = 1.f;
    float z  = 1.f;
    for (int i = test5->get_min_index(); i <= test5->get_max_index() ; ++i, z +=1.f)
    {
        for (int j = (*test5)[i].get_min_index(); j <= (*test5)[i].get_max_index() ; ++j, y +=1.f)
        {
            for (int k = (*test5)[i][j].get_min_index(); k <= (*test5)[i][j].get_max_index() ; ++k, x += 1.f)
            {
                (*test5)[i][j][k] = x * y *z;
            }
            x = 0.f;
        }
        y = 1.f;
    }

    return true;
}

bool SavvyWindow::test_display_array_1d()
{

    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->get_array_ptr("test1l");
    QString _name = pnl_workspace->get_array_name(1) + "_array_1d";

    display_array(test1_sptr, _name);

    return true;
}

bool SavvyWindow::test_display_array_1d_points()
{

    std::shared_ptr <stir::ArrayInterface> test_sptr = pnl_workspace->get_array_ptr("test1");
    QString _name = pnl_workspace->get_array_name(1) + "_array_1d_points";

    stir::Array<1, float> * tmp = dynamic_cast<stir::Array<1, float> * >(test_sptr.get());

    Display_manager *disp = new Display_manager(next_dataset_id, 1, this);
    ++next_dataset_id;
    disp->set_file_name(_name);

    QVector<double> y_values;
    QVector <double> x_values;

    savvy::decompose_Array1D_to_QVectors_XY(*tmp, x_values, y_values);
    if (is_null_ptr(disp))
        return false;
    disp->get_display()->set_display(x_values, y_values);

    append_to_mdi(disp);

    return true;
}

bool SavvyWindow::test_display_array_2d_in_1d_container()
{

    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_array_ptr("test2l");
    QString _name = pnl_workspace->get_array_name(3)+ "_array_2d_in_1d";

    stir::Array<2, float> * tmp = dynamic_cast<stir::Array<2, float> * >(test2_sptr.get());

    Display_manager * disp =  new Display_manager(next_dataset_id, 1, this);
    disp->set_file_name(_name);
    ++next_dataset_id;

    if (is_null_ptr(disp))
        return false;

    disp->get_display()->set_display(*tmp);
    append_to_mdi(disp);

    return true;
}

bool SavvyWindow::test_display_array_3d_in_1d_container()
{

    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_array_ptr("test3l");
    QString _name = pnl_workspace->get_array_name(5) + "_array_3d_in_1d";

    stir::Array<3, float> * tmp = dynamic_cast<stir::Array<3, float> * >(test2_sptr.get());

    Display_manager * disp =  new Display_manager(next_dataset_id, 1, this);
    disp->set_file_name(_name);
    ++next_dataset_id;

    if (is_null_ptr(disp))
        return false;

    disp->get_display()->set_display(*tmp);
    append_to_mdi(disp);

    return true;
}

bool SavvyWindow::test_display_array_2d()
{

    std::shared_ptr <stir::ArrayInterface> test_sptr = pnl_workspace->get_array_ptr("test2");
    QString _name = pnl_workspace->get_array_name(3) + "_array_2d";

    display_array(test_sptr, _name);

    return true;
}

bool SavvyWindow::test_display_array_1d_in_2d_container()
{

    std::shared_ptr <stir::ArrayInterface> test_sptr = pnl_workspace->get_array_ptr("test1");
    QString _name = pnl_workspace->get_array_name(1)+ "_array_2d_in_1d";

    stir::Array<1, float> * tmp = dynamic_cast<stir::Array<1, float> * >(test_sptr.get());

    Display_manager * disp =  new Display_manager(next_dataset_id, 2, this);
    disp->set_file_name(_name);
    ++next_dataset_id;

    if (is_null_ptr(disp))
        return false;

    disp->get_display()->set_display(*tmp, static_cast<int>(tmp->size()));
    append_to_mdi(disp);

    return true;
}
//!\todo
bool SavvyWindow::test_display_array_3d_in_2d_container()
{

    //    std::shared_ptr <stir::ArrayInterface> test_sptr = pnl_workspace->get_array_ptr(4);
    //    QString _name = pnl_workspace->get_array_name(4)+ "_array_3d_in_2d";

    //    stir::Array<3, float> * tmp = dynamic_cast<stir::Array<3, float> * >(test_sptr.get());

    //    Display_manager * disp =  new Display_manager(next_window_id, 2, this);
    //    disp->set_file_name(_name);
    //    ++next_window_id;

    //    if (is_null_ptr(disp))
    //        return false;

    //    disp->get_display()->set_display(*tmp);
    //    append_to_mdi(disp);

    return true;
}

bool SavvyWindow::test_display_array_3d()
{

    std::shared_ptr <stir::ArrayInterface> test_sptr = pnl_workspace->get_array_ptr(4);
    QString _name = pnl_workspace->get_array_name(4) + "_array_3d";

    display_array(test_sptr, _name);

    return true;
}


//bool SavvyWindow::test_display_2d_data_alt()
//{

//    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
//    QString n = pnl_workspace->get_current_name() + "_no_offset";
//    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

//    display_array(test2_sptr, n);

////        int size = test2->size();
////        QVector<QVector<double> > vtest2(size);

////        for (int i = 0; i < size; ++i)
////            vtest2[i].resize(size);

////        savvy::Array2QVector(*test2, vtest2);

////        //! \todo Display_container cannot be used for display outside of a wrapper
////        Display_manager *child = createDisplayManager(2);
////        child->set_file_name(n);

////        if (is_null_ptr(child))
////            return false;

////        child->set_display(&vtest2);

////        append_to_mdi(child);

//    return true;
//}

//bool SavvyWindow::test_display_2d_data_physical()
//{

//    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
//    QString n = pnl_workspace->get_current_name() + "_physical";
//    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

//    display_array(test2_sptr, n);

//    //    int size = test2->size();
//    //    QVector<QVector<double> > vtest2(size);

//    //    for (int i = 0; i < size; ++i)
//    //        vtest2[i].resize(size);

//    //    savvy::Array2QVector(*test2, vtest2);

//    //! \todo Display_container cannot be used for display outside of a wrapper
//    //    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    //    child->set_file_name(n);

//    //    if (is_null_ptr(child))
//    //        return false;

//    //    child->set_physical_display(vtest2,
//    //                                (*test2)[0].get_min_index(), (*test2).get_min_index(),
//    //            0.5, 0.5);

//    //    append_to_mdi(child);

//    return true;
//}

//bool SavvyWindow::test_display_2d_data_alt_not_square()
//{

//    int row_size = 60;
//    int row_num = 120;

//    int start_x= -row_size/2;
//    int start_y = -row_num/2;
//    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
//                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));

//    std::shared_ptr <stir::ArrayInterface> test3_sptr = pnl_workspace->create_array_ptr<2>(range, "test3");
//    stir::Array<2, float>* test3 = dynamic_cast<stir::Array<2, float>* >(test3_sptr.get());

//    for (int i = test3->get_min_index(); i <= test3->get_max_index() ; ++i)
//        for (int j = (*test3)[i].get_min_index(); j <= (*test3)[i].get_max_index() ; ++j)
//        {
//            float f = sqrt(i*i + j*j);
//            (*test3)[i][j] = f;
//        }

//    QString n = pnl_workspace->get_current_name() + "_vertical";
//    display_array(test3_sptr, n);
//    //    int num_row = test3->size();
//    //    int size_row = (*test3)[0].size();
//    //    QVector<QVector<double> > vtest3(num_row);

//    //    for (int i = 0; i < num_row; ++i)
//    //        vtest3[i].resize(size_row);

//    //    savvy::Array2QVector(*test3, vtest3);

//    //    QVector<double> svtest3;
////       savvy::serialize_QVector(vtest3, svtest3);

//    //! \todo Display_container cannot be used for display outside of a wrapper
//    //    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));

//    //    child->set_file_name(n);

//    //    if (is_null_ptr(child))
//    //        return false;

//    //    child->set_display(svtest3, size_row);

//    //    append_to_mdi(child);

//    return true;
//}

//bool SavvyWindow::test_display_2d_data_physical_not_square()
//{
//    int row_size = 120;
//    int row_num = 60;

//    int start_x= -row_size/2;
//    int start_y = -row_num/2;

//    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
//                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));
//    std::shared_ptr <stir::ArrayInterface> test4_sptr = pnl_workspace->create_array_ptr<2>(range, "test4");
//    stir::Array<2, float>* test4 = dynamic_cast<stir::Array<2, float>* >(test4_sptr.get());

//    for (int i = test4->get_min_index(); i <= test4->get_max_index() ; ++i)
//        for (int j = (*test4)[i].get_min_index(); j <= (*test4)[i].get_max_index() ; ++j)
//        {
//            float f = sqrt(i*i + j*j);
//            (*test4)[i][j] = f;
//        }
//    QString n = pnl_workspace->get_current_name() + "_horizontal";
//    display_array(test4_sptr, n);
//    //    QVector<QVector<double> > vtest4(test4->size());

//    //    for (int i = 0; i < test4->size(); ++i)
//    //        vtest4[i].resize((*test4)[0].size());

//    //    savvy::Array2QVector(*test4, vtest4);
//    //! \todo Display_container cannot be used for display outside of a wrapper
//    //    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    //    QString n = pnl_workspace->get_current_name() + "_horizontal";
//    //    child->set_file_name(n);

//    //    if (is_null_ptr(child))
//    //        return false;

//    //    child->set_physical_display(vtest4,
//    //                                (*test4)[0].get_min_index(), (*test4).get_min_index(),
//    //            0.5, 0.5);

//    //    append_to_mdi(child);

//    return true;
//}

//bool SavvyWindow::test_display_3d_data()
//{

//    const IndexRange<3> range(stir::Coordinate3D<int>(-30,-60, -60),stir::Coordinate3D<int>(29,59, 59));
//    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->create_array_ptr<3>(range, "test5");
//    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

//    for (int i = test5->get_min_index(); i <= test5->get_max_index() ; ++i)
//        for (int j = (*test5)[i].get_min_index(); j <= (*test5)[i].get_max_index() ; ++j)
//            for (int k = (*test5)[i][j].get_min_index(); k <= (*test5)[i][j].get_max_index() ; ++k)
//            {
//                float f = sqrt(i*i + k*k + j*j);
//                if( f != 0)
//                    (*test5)[i][j][k] = sin(f) / f;
//                else
//                    (*test5)[i][j][k] = 1;
//            }

//    QString n = pnl_workspace->get_current_name() + "_indiced";
//    display_array(test5_sptr, n);

//    return true;
//}

//bool SavvyWindow::test_display_3d_data_alt()
//{
////    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->get_current_array_ptr();
////    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

////    QString n = pnl_workspace->get_current_name() + "_indiced_alt";
////    QVector<QVector<QVector<double> > > vtmp;
////    Display_manager* manager = createDisplayManager();
////    manager->set_file_name(n);

////    savvy::Array2QVector(*test5, vtmp);

////    Display_container_3d * disp =
////            dynamic_cast<Display_container_3d*>(manager->get_display());

////    disp->set_display(vtmp);

////    append_to_mdi(manager);
////    return true;
//}

/*
 *
 *  ABOUT
 *
 */
void SavvyWindow::on_actionAbout_triggered()
{
    AboutDialog* about = new AboutDialog(this);
    about->exec();
}

void SavvyWindow::on_actionAbout_Plugins_triggered()
{
    QSettings settings;
    if(settings.contains("PluginsPath"))
        pluginsDir.setPath(settings.value("PluginsPath").toString());

    PlugInsDialog dialog(pluginsDir.path(), pluginFileNames, this);
    dialog.exec();
}

void SavvyWindow::loadPlugins()
{
    foreach (QObject *plugin, QPluginLoader::staticInstances())
        populateMenus(plugin);

    QSettings settings;
    if(settings.contains("PluginsPath"))
        pluginsDir = settings.value("PluginsPath").toString();
    else
        pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            populateMenus(plugin);
            pluginFileNames += fileName;
        }
    }
}

void SavvyWindow::populateMenus(QObject *plugin)
{
    ExternalInterface *iProc = qobject_cast<ExternalInterface *>(plugin);
    iProc->link_workspace(pnl_workspace);
    if (iProc)
        addToMenu(iProc, iProc->get_name(), ui->menuPlugins);
}

void SavvyWindow::addToMenu(ExternalInterface *plugin, const QString text,
                            QMenu *menu)
{

    QAction *action = new QAction(text, plugin);
    loaded_plugins.append(action);

    connect(loaded_plugins.back(), SIGNAL(triggered()), plugin, SLOT(show()));
    menu->addAction(action);
}

void SavvyWindow::on_actionWindow_Level_triggered()
{
    if(DisplayInterface* active =
            qobject_cast<DisplayInterface *>(ui->mdiArea->activeSubWindow()->widget()))
    {
        if(dc_contrast->isHidden())
        {
            dc_contrast->show();
        }
        else
        {
            dc_contrast->hide();
        }

        if (!dc_contrast->isHidden())
        {
            ctrl_levels->unsetContainer();
            ctrl_levels->setContainer(active);
            //disconnect(active, &Screen_manager::activeScreenUpdated, my_histogram, &BarScreen::replot_me);
            //my_histogram->setScreen(active);
            //connect(active, &Screen_manager::activeScreenUpdated, my_histogram, &BarScreen::replot_me);
        }
    }
}

void SavvyWindow::on_actionProcess_Stack_triggered()
{
    StackProcessorUI *r = new StackProcessorUI(pnl_workspace);
    r->show();
}
