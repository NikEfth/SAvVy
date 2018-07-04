#include "src/include/savvy.h"
#include "savvywindow.h"
#include "ui_savvywindow.h"
#include "src/IO/io_manager.h"
#include "src/display/display_container_1d.h"
#include "src/display/display_container_2d.h"
#include "savvy_settings.h"

#include "stir/is_null_ptr.h"
#include "stir/common.h"
#include "stir/Coordinate2D.h"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

USING_NAMESPACE_STIR

SavvyWindow::SavvyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SavvyWindow)
{
    ui->setupUi(this);
    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());

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

    Array<3, float>* tmp
            = io_manager::open_array(fileName.toStdString(), num_of_data);

    if(!is_null_ptr(tmp))
    {
        //error opening file!!
    }

    Display_container *child = createMdiChild();

    if (is_null_ptr(child))
        return false;

    //           if(dynamic_cast<Display_container_1d*>(child)->set_array(tmp))
    //               append_to_workspace(child);

    pnl_opened_files->appendToOpenedList(child);

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
    pnl_opened_files->setEnabled(hasMdiChild);

    if (! stir::is_null_ptr(activeSubWindow))
    {
        Display_container * active_display_container =
                qobject_cast<Display_container *>(activeSubWindow->widget());

        if (active_display_container != previous_active)
        {
            pnl_opened_file_controls->show_panel(active_display_container->get_num_dimensions());
            pnl_opened_files->set_active(active_display_container->get_my_id());

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

Display_container *SavvyWindow::createMdiChild(int num_dims)
{
    next_window_id++;

    if(num_dims == 1)
        return new Display_container_1d(next_window_id-1,1, this);
    else if(num_dims == 2)
        return new Display_container_2d(next_window_id-1, 2, this);

    return NULL;
}

QMdiSubWindow *SavvyWindow::findMdiChild(const QString &_id) const
{
    //QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    for(QMdiSubWindow *window: ui->mdiArea->subWindowList()) {
        Display_container *mdiChild = qobject_cast<Display_container *>(window->widget());
        if (QString::number(mdiChild->get_my_id()) == _id)
            return window;
    }
    return 0;
}

bool SavvyWindow::append_to_workspace(Display_container *child,
                                      bool prepend_to_recent,
                                      bool minimized)
{
    QObject::connect(child, &Display_container::aboutToClose, this, &SavvyWindow::remove_from_workspace);
    //    QObject::connect(child, SIGNAL(aboutToClose()), this,SLOT(removeFromGroupedList()));
    //   QObject::connect(child, SIGNAL(updatedContainer()), this,SLOT(updateMenus()));

    ui->mdiArea->addSubWindow(child)->setWindowTitle(child->get_file_name());
    ui->mdiArea->setFocusPolicy(Qt::StrongFocus);
    pnl_opened_files->appendToOpenedList(child);

    if (!minimized)
        child->show();
    else
        child->showMinimized();

    return true;
}

void SavvyWindow::remove_from_workspace()
{
    // Find who send this
    Display_container* src = qobject_cast<Display_container *>(sender());

    pnl_opened_files->removeFromOpenedList(src);
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
    pnl_opened_files = new Panel_opened_files(this);
    connect(pnl_opened_files, &Panel_opened_files::double_clicked_item,
            this, &SavvyWindow::focus_sub_window);
    // TODO: The other way around.
    dc_opened_files->setWidget(pnl_opened_files);

    dc_opened_file_controls = new QDockWidget("Controls", this);
    pnl_opened_file_controls = new Panel_opened_file_controls(this);
    dc_opened_file_controls->setWidget(pnl_opened_file_controls);

    this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dc_contrast);
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dc_tool_manager);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_files);
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

    tileVerticalAct = new QAction(tr("Tile Vertically"), this);
    tileVerticalAct->setStatusTip(tr("Tile the windows vertically"));
    connect(tileVerticalAct, &QAction::triggered, this, &SavvyWindow::tileSubWindowsVertically);
    ui->menuWindow->addAction(tileVerticalAct);

    tileHorizontalAct = new QAction(tr("Tile Horizontally"), this);
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

    closeAllAct = new QAction(tr("&Close All"), this);
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

}

bool SavvyWindow::test_display_1d_data()
{
    Display_container_1d *child = dynamic_cast<Display_container_1d *>(createMdiChild());
    child->set_file_name("test1 - Indiced dimensions");

    if (is_null_ptr(child))
        return false;

    const IndexRange<1> range(-60,60);
    stir::Array<1, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
    {
        if(i != 0)
        {
            float f = static_cast<float>(i);
            test1[i] = sin(f) / f;
        }
        else
            test1[i] = 1;
    }

    QVector<double> vtest1(test1.size());

    savvy::Array2QVector(test1, vtest1);

    child->set_display(vtest1, test1.get_min_index());

    append_to_workspace(child);

    return true;
}

bool SavvyWindow::test_display_1d_data_physical()
{
    Display_container_1d *child = dynamic_cast<Display_container_1d *>(createMdiChild());
    child->set_file_name("test1 - Physical dimensions");

    if (is_null_ptr(child))
        return false;

    const IndexRange<1> range(-60,60);
    stir::Array<1, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
    {
        if(i != 0)
        {
            float f = static_cast<float>(i);
            test1[i] = sin(f) / f;
        }
        else
            test1[i] = 1;
    }

    QVector<double> vtest1(test1.size());

    savvy::Array2QVector(test1, vtest1);

    child->set_physical_display(vtest1,
                                test1.get_min_index(), -30, 30);

    append_to_workspace(child);

    return true;
}

bool SavvyWindow::test_display_2d_data()
{
    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createMdiChild(2));
    child->set_file_name("test2 - Indiced dimensions");

    if (is_null_ptr(child))
        return false;

    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
        {
            float f = sqrt(i*i + j*j);
            if( f != 0)
                test1[i][j] = sin(f) / f;
            else
                test1[i][j] = 1;
        }

    int size = test1.size();
    QVector<QVector<double> > vtest1(size);

    for (int i = 0; i < size; ++i)
        vtest1[i].resize(size);

    savvy::Array2QVector(test1, vtest1);

    child->set_display(vtest1,
                       test1.get_min_index(), test1[0].get_min_index());

    append_to_workspace(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_alt()
{
    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createMdiChild(2));
    child->set_file_name("test2 - Indiced dimensions_alt");

    if (is_null_ptr(child))
        return false;

    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
        {
            float f = sqrt(i*i + j*j);
            if( f != 0)
                test1[i][j] = sin(f) / f;
            else
                test1[i][j] = 1;
        }

    int size = test1.size();
    QVector<QVector<double> > vtest1(size);

    for (int i = 0; i < size; ++i)
        vtest1[i].resize(size);

    savvy::Array2QVector(test1, vtest1);

    QVector<double> svtest1;
    savvy::serialize_QVector(vtest1, svtest1);

    child->set_display(svtest1, size);

    append_to_workspace(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_alt_not_square()
{
    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createMdiChild(2));
    child->set_file_name("test2 - Indiced dimensions_alt");

    if (is_null_ptr(child))
        return false;

    int row_size = 60;
    int row_num = 120;

    int start_x= -row_size/2;
    int start_y = -row_num/2;
    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
            test1[i][j] = j;

    int num_row = test1.size();
    int size_row = test1[0].size();
    QVector<QVector<double> > vtest1(num_row);

    for (int i = 0; i < num_row; ++i)
        vtest1[i].resize(size_row);

    savvy::Array2QVector(test1, vtest1);

    QVector<double> svtest1;
    savvy::serialize_QVector(vtest1, svtest1);

    child->set_display(svtest1, size_row,
                       test1[0].get_min_index(), test1.get_min_index());

    append_to_workspace(child);

    return true;
}


bool SavvyWindow::test_display_2d_data_physical()
{
    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createMdiChild(2));
    child->set_file_name("test2 - physical dimensions");

    if (is_null_ptr(child))
        return false;

    const IndexRange<2> range(stir::Coordinate2D<int>(-60, -60),
                              stir::Coordinate2D<int>(59,59));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
        {
            float f = sqrt(i*i + j*j);
            if( f != 0)
                test1[i][j] = sin(f) / f;
            else
                test1[i][j] = 1;
        }

    int size = test1.size();
    QVector<QVector<double> > vtest1(size);

    for (int i = 0; i < size; ++i)
        vtest1[i].resize(size);

    savvy::Array2QVector(test1, vtest1);

    child->set_physical_display(vtest1,
                                test1[0].get_min_index(), test1.get_min_index(),
            0.5, 0.5);

    append_to_workspace(child);

    return true;
}

bool SavvyWindow::test_display_2d_data_physical_not_square()
{
    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createMdiChild(2));
    child->set_file_name("test2 - physical dimensions");

    if (is_null_ptr(child))
        return false;

    int row_size = 120;
    int row_num = 60;

    int start_x= -row_size/2;
    int start_y = -row_num/2;

    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
        {
            float f = i;
            test1[i][j] = f;
        }

    QVector<QVector<double> > vtest1(test1.size());

    for (int i = 0; i < test1.size(); ++i)
        vtest1[i].resize(test1[0].size());

    savvy::Array2QVector(test1, vtest1);

    child->set_physical_display(vtest1,
                                test1[0].get_min_index(), test1.get_min_index(),
            0.5, 0.5);

    append_to_workspace(child);

    return true;
}


int SavvyWindow::ask(QString question)
{
    QMessageBox msgBox;
    msgBox.setText(question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    return msgBox.exec();
}
