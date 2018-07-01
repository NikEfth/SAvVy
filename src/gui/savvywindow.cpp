#include "savvywindow.h"
#include "ui_savvywindow.h"
#include "src/IO/io_manager.h"
#include "src/display/display_container_1d.h"

#include "stir/is_null_ptr.h"
#include "stir/FilePath.h"
#include "stir/common.h"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

using namespace stir;

SavvyWindow::SavvyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SavvyWindow)
{
    ui->setupUi(this);
    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    // create docks, toolbars, etc…
    restoreState(settings.value("mainWindowState").toByteArray());

    QDir init_dir = QDir::homePath();
    initial_open_path = init_dir.absolutePath();
    this->setWindowIcon(QIcon(":resources/icons8-microbeam-radiation-therapy-80.png"));

    next_window_id = 1;

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
    {
        // Some basic checks
        stir::FilePath current_file(fileName.toStdString()) ;

        if (current_file.get_extension() == "hv")
        {

            int num_of_data    = 0;
            int array_x_dim     = 0;
            int array_y_dim     = 0;
            int array_z_dim     = 0;

            Array<3, float>* tmp
                    = io_manager::open_array(fileName.toStdString(), num_of_data,
                                             array_x_dim,
                                             array_y_dim,
                                             array_z_dim);

            if(!is_null_ptr(tmp))
            {
                //error opening file!!
            }

            QWidget *child = createMdiChild();

            if (is_null_ptr(child))
                return;

            //           if(dynamic_cast<Display_container_1d*>(child)->set_array(tmp))
            //               append_to_workspace(child);
        }
    }
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

}


QWidget *SavvyWindow::createMdiChild(int num_dims)
{
    if(num_dims == 1)
        return new Display_container_1d(this);

    new std::nullptr_t;
}

bool SavvyWindow::append_to_workspace(QWidget *child,
                                      bool prepend_to_recent,
                                      bool minimized)
{
    //    QObject::connect(child, SIGNAL(aboutToClose()), this,SLOT(removeFromOpenedList()));
    //    QObject::connect(child, SIGNAL(aboutToClose()), this,SLOT(removeFromGroupedList()));
    //    QObject::connect(child, SIGNAL(updatedContainer()), this,SLOT(updateMenus()));

    ui->mdiArea->addSubWindow(child);
    ui->mdiArea->setFocusPolicy(Qt::StrongFocus);

    if (!minimized)
        child->show();
    else
        child->showMinimized();

    //    appendToOpenedList(child);

    //    if(prepend_to_recent)
    //        MainWindow::prependToRecentFiles(child->getFullFilePath());

    return true;
}




/*
 *
 *
 *      GUI STAFF
 *
 */

void SavvyWindow::create_interface()
{
    create_docks();

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
    //    toolMan = new ToolManager(dc_tool_manager);
    dc_opened_files->setWidget(pnl_opened_files);

    dc_opened_file_controls = new QDockWidget("Controls", this);
    pnl_opened_file_controls = new Panel_opened_file_controls(this);
    //     //    toolMan = new ToolManager(dc_tool_manager);
    dc_opened_file_controls->setWidget(pnl_opened_file_controls);

    this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, dc_contrast);
    this->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dc_tool_manager);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_files);
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dc_opened_file_controls);
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

    all_tests = ask(QString("A sinc function is displayed."));

}

bool SavvyWindow::test_display_1d_data()
{
    Display_container_1d *child = dynamic_cast<Display_container_1d *>(createMdiChild());

    if (is_null_ptr(child))
        return false;

    const IndexRange<1> range(-30,30);
    stir::Array<1, float> test1(range);

    for (int i = -30; i <= 30 ; ++i)
    {
        if(i != 0)
            test1[i] = sin(i) / i;
        else
            test1[i] = 1;
    }

    QVector<double> vtest1(test1.size());

    io_manager::Array2QVector(test1, vtest1);

    child->set_display(vtest1, -30);

    append_to_workspace(child);

    return true;
}

int SavvyWindow::ask(QString question)
{
    QMessageBox msgBox;
    msgBox.setText(question);
    msgBox.setInformativeText("Is this correct?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
   return msgBox.exec();
}
