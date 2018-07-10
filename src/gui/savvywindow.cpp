#include "src/include/savvy.h"
#include "savvywindow.h"
#include "ui_savvywindow.h"
#include "src/IO/io_manager.h"
#include "src/display/display_container_1d.h"
#include "src/display/display_container_2d.h"
#include "src/display/display_container_3d.h"
#include "savvy_settings.h"
#include "aboutdialog.h"

#include "stir/is_null_ptr.h"
#include "stir/common.h"
#include "stir/Coordinate2D.h"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QInputDialog>

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

    next_window_id = 0;

    create_interface();

    // This connection will update the window on every interaction
    // It is the main "refresh all" interface function
    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            this, &SavvyWindow::updateGUI);
}

SavvyWindow::~SavvyWindow()
{
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
                                          tr("All Files (*.*)"
                                             "STIR Interfile Header (*.hs);;"
                                             "STIR Image Header (*.hv);;"
                                             "Dicom Images (*.dicom);;"
                                             ));

    if (fileNames.size() == 0)
        return;

    for (QString fileName : fileNames)
        open_file(fileName);
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

bool SavvyWindow::open_file(const QString& fileName)
{
    int num_of_data = 1;

    //    std::shared_ptr <stir::ArrayInterface> tmp_sptr =
    //            pnl_workspace->open_array_ptr(fileName.toStdString());

    //    Array<3, float>* tmp
    //            = io_manager::open_array(fileName.toStdString(), num_of_data);

    //    if(!is_null_ptr(tmp_sptr))
    //    {
    //        //error opening file!!
    //    }

    //    display_array(tmp_sptr, pnl_workspace->get_current_name());


    prependToRecentFiles(fileName);

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
    if (!hasMdiChild)
        pnl_opened_file_controls->show_panel(0);
    pnl_displayed_files->setEnabled(hasMdiChild);
    ui->menuSlice_Annotations->setEnabled(hasMdiChild);
    ui->menuAdjust->setEnabled(hasMdiChild);

    if (! stir::is_null_ptr(activeSubWindow))
    {
        DisplayInterface * active_display_container =
                dynamic_cast<DisplayInterface *>(activeSubWindow->widget());

        if (active_display_container != previous_active)
        {
            pnl_opened_file_controls->show_panel(active_display_container->get_num_dimensions());
            pnl_displayed_files->set_active(active_display_container->get_my_id());

            previous_active = active_display_container;
        }
    }

    //    int current_view_mode = ui->mdi
    //    pnl_opened_file_controls->show_panel();

}

void SavvyWindow::focus_sub_window(QString _id)
{
    if (QMdiSubWindow *existing = findMdiChild(_id)) {
        ui->mdiArea->setActiveSubWindow(existing);
    }
}

Display_container *SavvyWindow::createDisplayContainer(int num_dims)
{
    next_window_id++;

    if(num_dims == 1)
        return new Display_container_1d(next_window_id-1,1, this);
    else if(num_dims == 2)
        return new Display_container_2d(next_window_id-1, 2, this);
    else if (num_dims == 3)
        return new Display_container_3d(next_window_id-1, 3, this);

    return NULL;
}

Display_manager *SavvyWindow::createDisplayManager(int num_dims)
{
    next_window_id++;

    if(num_dims == 1)
        return new Display_manager(next_window_id-1,1, this);
    else if(num_dims == 2)
        return new Display_manager(next_window_id-1, 2, this);
    else if (num_dims == 3)
        return new Display_manager(next_window_id-1, 3, this);

    return NULL;
}

QMdiSubWindow *SavvyWindow::findMdiChild(const QString &_id) const
{

    for(QMdiSubWindow *window: ui->mdiArea->subWindowList()) {
        Display_container *mdiChild = qobject_cast<Display_container *>(window->widget());
        if (QString::number(mdiChild->get_my_id()) == _id)
            return window;
    }
    return 0;

}

bool SavvyWindow::append_to_mdi(Display_container *child,
                                bool prepend_to_recent,
                                bool minimized)
{
    QObject::connect(child, &Display_container::aboutToClose, this, &SavvyWindow::remove_from_mdi);

    ui->mdiArea->addSubWindow(child);
    ui->mdiArea->setFocusPolicy(Qt::StrongFocus);
    pnl_displayed_files->appendToOpenedList(child);

    if (!minimized)
        child->show();
    else
        child->showMinimized();

    return true;
}

bool SavvyWindow::append_to_mdi(Display_manager *child,
                                bool prepend_to_recent,
                                bool minimized)
{
    QObject::connect(child, &Display_manager::aboutToClose, this, &SavvyWindow::remove_from_mdi);

    ui->mdiArea->addSubWindow(child);
    ui->mdiArea->setFocusPolicy(Qt::StrongFocus);
    pnl_displayed_files->appendToOpenedList(child);

    if (!minimized)
        child->show();
    else
        child->showMinimized();

    return true;
}

void SavvyWindow::remove_from_mdi()
{
    // Find who send this
    DisplayInterface* src =
            dynamic_cast<DisplayInterface *>(sender());

    pnl_displayed_files->removeFromOpenedList(src);
    // Disconnect from tool
    //            toolMan->unsetScreen();
    // Disconnect from contrast window
    //            contrastMan->unsetScreen();
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
        Display_container *container = createDisplayContainer(dims);
        container->set_file_name(_name);

        if (is_null_ptr(container))
            return;

        stir::Array<1, float>* tmp = dynamic_cast<stir::Array<1, float>* >(_array.get());
        Display_container_1d *child = dynamic_cast<Display_container_1d *>(container);

        QVector<double> vtmp;

        savvy::Array2QVector(*tmp, vtmp);
        // With offset
        //        child->set_display(vtmp,
        //                           tmp->get_min_index());

        child->set_display(vtmp);
        append_to_mdi(container);
        return;
    }
    case 2:
    {
        Display_container *container = createDisplayContainer(dims);
        container->set_file_name(_name);

        if (is_null_ptr(container))
            return;

        stir::Array<2, float>* tmp = dynamic_cast<stir::Array<2, float>* >(_array.get());
        Display_container_2d *child = dynamic_cast<Display_container_2d *>(container);

//        QVector<QVector<double> > vtmp;
//        savvy::Array2QVector(*tmp, vtmp);
//                child->set_display(vtmp);

        //        child->set_display(vtmp,
        //                           tmp->get_min_index(),
        //                           (*tmp)[0].get_min_index());
        child->set_display(*tmp);
        append_to_mdi(container);
        return;
    }
    case 3:
    {
        stir::Array<3, float>* tmp = dynamic_cast<stir::Array<3, float>* >(_array.get());
        Display_manager* manager = createDisplayManager();
        manager->set_file_name(_name);

        Display_container_3d * disp =
                dynamic_cast<Display_container_3d*>(manager->get_display());

        disp->set_display(*tmp);

        append_to_mdi(manager);
        return;
    }
    default:
        //! \todo throw error
        break;
    }
}


void SavvyWindow::on_actionDuplicate_triggered()
{
    Display_container* active =
            qobject_cast<Display_container *>(ui->mdiArea->activeSubWindow()->widget());


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
    //    contrastMan = new ContrastMan(this);
    //    dc_contrast->setWidget(contrastMan);

    dc_contrast->setFloating(true);
    dc_contrast->setVisible(false);

    dc_tool_manager = new QDockWidget("Tools", this);

    //    toolMan = new ToolManager(dc_tool_manager);
    //    dc_right->setWidget(toolMan);

    dc_opened_files = new QDockWidget("Opened Files", this);
    pnl_workspace = new Workspace(this);
    connect(ui->mdiArea, &QMdiArea::subWindowActivated,
            pnl_workspace, &Workspace::updateGUI);
    connect(pnl_workspace, &Workspace::display_current,
            this, &SavvyWindow::display_array);
    dc_opened_files->setWidget(pnl_workspace);

    dc_displayed_files = new QDockWidget("Displayed Files", this);
    pnl_displayed_files = new Panel_displayed_files(this);
    connect(pnl_displayed_files, &Panel_displayed_files::double_clicked_item,
            this, &SavvyWindow::focus_sub_window);
    dc_displayed_files->setWidget(pnl_displayed_files);

    dc_opened_file_controls = new QDockWidget("Controls", this);
    pnl_opened_file_controls = new Panel_opened_file_controls(this);
    dc_opened_file_controls->setWidget(pnl_opened_file_controls);

    this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dc_contrast);
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dc_tool_manager);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_files);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_displayed_files);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_file_controls);
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

void SavvyWindow::tileSubWindowsVertically()
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

void SavvyWindow::tileSubWindowsHorizontally()
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

void SavvyWindow::shadeSubWindows()
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

void SavvyWindow::prependToRecentFiles(const QString &fileName)
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

void SavvyWindow::setRecentFilesVisible(bool visible)
{
    recentFileSubMenuAct->setVisible(visible);
    recentFileSeparator->setVisible(visible);
}

void SavvyWindow::updateRecentFileActions()
{
    QSettings settings;

    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i = 0;
    for ( ; i < count; ++i) {
        const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
        recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
        recentFileActs[i]->setData(recentFiles.at(i));
        recentFileActs[i]->setVisible(true);
    }
    for ( ; i < MaxRecentFiles; ++i)
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

    all_tests = test_display_1d_data();

    //all_tests = ask(QString("Is this the output of a sinc function? "));

    all_tests = test_display_1d_data_physical();

    //all_tests = ask(QString("Has the range of x axis reduced half? "));

    all_tests = test_display_2d_data();

    //all_tests = ask(QString("Does the image look like a ripple? "));

    all_tests = test_display_2d_data_alt();

    //all_tests = ask(QString("Was this the same as the previous? "));

    all_tests = test_display_2d_data_physical();

    //all_tests = ask(QString("Has the range of the image reduced half? "));

    all_tests = test_display_2d_data_alt_not_square();

    //all_tests = ask(QString("Was this the same as the previous? "));

    all_tests = test_display_2d_data_physical_not_square();

    //all_tests = ask(QString("Has the range of the image reduced half? "));

    all_tests = test_display_3d_data();

    all_tests = test_display_3d_data_alt();

}

int SavvyWindow::ask(QString question)
{
    QMessageBox msgBox;
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}


bool SavvyWindow::test_display_1d_data()
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

    QString n = pnl_workspace->get_current_name() + "_indiced";

    display_array(test1_sptr, n);

    return true;
}

bool SavvyWindow::test_display_1d_data_physical()
{

    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->get_array_ptr(0);
    QString n = pnl_workspace->get_current_name() + "_physical";

    display_array(test1_sptr, n);

    return true;
}

bool SavvyWindow::test_display_2d_data()
{

    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->create_array_ptr<2>(range, "test2");
    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

    for (int i = test2->get_min_index(); i <= test2->get_max_index() ; ++i)
        for (int j = (*test2)[i].get_min_index(); j <= (*test2)[i].get_max_index() ; ++j)
        {
            float f = sqrt(i*i + j*j);
            if( f != 0)
                (*test2)[i][j]  = sin(f) / f;
            else
                (*test2)[i][j] = 1;
        }

    QString n = pnl_workspace->get_current_name() + "_indiced";
    display_array(test2_sptr, n);

    return true;
}

bool SavvyWindow::test_display_2d_data_alt()
{

    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
    QString n = pnl_workspace->get_current_name() + "_no_offset";
    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

    int size = test2->size();
    QVector<QVector<double> > vtest2(size);

    for (int i = 0; i < size; ++i)
        vtest2[i].resize(size);

    savvy::Array2QVector(*test2, vtest2);

    QVector<double> svtest2;
    savvy::serialize_QVector(vtest2, svtest2);

    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
    child->set_file_name(n);

    if (is_null_ptr(child))
        return false;

    child->set_display(svtest2, size);

    append_to_mdi(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_physical()
{

    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
    QString n = pnl_workspace->get_current_name() + "_physical";
    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());


    int size = test2->size();
    QVector<QVector<double> > vtest2(size);

    for (int i = 0; i < size; ++i)
        vtest2[i].resize(size);

    savvy::Array2QVector(*test2, vtest2);

    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
    child->set_file_name(n);

    if (is_null_ptr(child))
        return false;

    child->set_physical_display(vtest2,
                                (*test2)[0].get_min_index(), (*test2).get_min_index(),
            0.5, 0.5);

    append_to_mdi(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_alt_not_square()
{

    int row_size = 60;
    int row_num = 120;

    int start_x= -row_size/2;
    int start_y = -row_num/2;
    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));

    std::shared_ptr <stir::ArrayInterface> test3_sptr = pnl_workspace->create_array_ptr<2>(range, "test3");
    stir::Array<2, float>* test3 = dynamic_cast<stir::Array<2, float>* >(test3_sptr.get());

    for (int i = test3->get_min_index(); i <= test3->get_max_index() ; ++i)
        for (int j = (*test3)[i].get_min_index(); j <= (*test3)[i].get_max_index() ; ++j)
            (*test3)[i][j] = j;

    int num_row = test3->size();
    int size_row = (*test3)[0].size();
    QVector<QVector<double> > vtest3(num_row);

    for (int i = 0; i < num_row; ++i)
        vtest3[i].resize(size_row);

    savvy::Array2QVector(*test3, vtest3);

    QVector<double> svtest3;
    savvy::serialize_QVector(vtest3, svtest3);

    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
    QString n = pnl_workspace->get_current_name() + "_vertical";
    child->set_file_name(n);

    if (is_null_ptr(child))
        return false;

    child->set_display(svtest3, size_row,
                       (*test3)[0].get_min_index(), (*test3).get_min_index());

    append_to_mdi(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_physical_not_square()
{
    int row_size = 120;
    int row_num = 60;

    int start_x= -row_size/2;
    int start_y = -row_num/2;

    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));
    std::shared_ptr <stir::ArrayInterface> test4_sptr = pnl_workspace->create_array_ptr<2>(range, "test4");
    stir::Array<2, float>* test4 = dynamic_cast<stir::Array<2, float>* >(test4_sptr.get());

    for (int i = test4->get_min_index(); i <= test4->get_max_index() ; ++i)
        for (int j = (*test4)[i].get_min_index(); j <= (*test4)[i].get_max_index() ; ++j)
        {
            float f = i;
            (*test4)[i][j] = f;
        }

    QVector<QVector<double> > vtest4(test4->size());

    for (int i = 0; i < test4->size(); ++i)
        vtest4[i].resize((*test4)[0].size());

    savvy::Array2QVector(*test4, vtest4);

    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
    QString n = pnl_workspace->get_current_name() + "_horizontal";
    child->set_file_name(n);

    if (is_null_ptr(child))
        return false;

    child->set_physical_display(vtest4,
                                (*test4)[0].get_min_index(), (*test4).get_min_index(),
            0.5, 0.5);

    append_to_mdi(child);

    return true;
}

bool SavvyWindow::test_display_3d_data()
{

    const IndexRange<3> range(stir::Coordinate3D<int>(-30,-60, -60),stir::Coordinate3D<int>(29,59, 59));
    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->create_array_ptr<3>(range, "test5");
    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

    for (int i = test5->get_min_index(); i <= test5->get_max_index() ; ++i)
        for (int j = (*test5)[i].get_min_index(); j <= (*test5)[i].get_max_index() ; ++j)
            for (int k = (*test5)[i][j].get_min_index(); k <= (*test5)[i][j].get_max_index() ; ++k)
            {
                float f = sqrt(i*i + k*k + j*j);
                if( f != 0)
                    (*test5)[i][j][k] = sin(f) / f;
                else
                    (*test5)[i][j][k] = 1;
            }

    QString n = pnl_workspace->get_current_name() + "_indiced";
    display_array(test5_sptr, n);

    return true;
}

bool SavvyWindow::test_display_3d_data_alt()
{
    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->get_current_array_ptr();
    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());

    QString n = pnl_workspace->get_current_name() + "_indiced_alt";
    QVector<QVector<QVector<double> > > vtmp;
    Display_manager* manager = createDisplayManager();
     manager->set_file_name(n);

    savvy::Array2QVector(*test5, vtmp);

    Display_container_3d * disp =
            dynamic_cast<Display_container_3d*>(manager->get_display());

    disp->set_display(vtmp);

    append_to_mdi(manager);
    return true;
}

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
