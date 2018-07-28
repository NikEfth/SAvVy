#include "savvy_settings.h"
#include "ui_savvy_settings.h"
#include "src/display/common_display.h"

#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>
#include <QSettings>
#include <QHBoxLayout>

#include <cmath>

GeneralSettings::GeneralSettings(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);

    QTabWidget *mainTab = new QTabWidget(this);

    {
        QWidget* defaultsPage = new QWidget(this);
        QVBoxLayout* defaultsPageLayout = new QVBoxLayout(defaultsPage);
        //        defaultsPageLayout->setContentsMargins(1,1,1,1);
        defaultsPage->setLayout(defaultsPageLayout);

        //
        autoPlotOpenedFiles = new QCheckBox("Automatically plot opened files. ", defaultsPage);
        defaultsPageLayout->addWidget(autoPlotOpenedFiles);

        autoContrast = new QCheckBox("Automatically optimise contrast when opening images. ", defaultsPage);
        defaultsPageLayout->addWidget(autoContrast);

        defaultAxis = new QCheckBox("Show axis by default ", defaultsPage);
        defaultsPageLayout->addWidget(defaultAxis);

        defaultTabbedMode = new QCheckBox("Use Tabbed view mode by default ", defaultsPage);
        defaultsPageLayout->addWidget(defaultTabbedMode);

        QHBoxLayout* hLayout = new QHBoxLayout(defaultsPage);
        pluginsPath = new QLineEdit(this);
        QLabel * lbl_pluginsPath = new QLabel("Plugins path: ");
        hLayout->addWidget(lbl_pluginsPath);
        hLayout->addWidget(pluginsPath);
        defaultsPageLayout->addItem(hLayout);


        defaultsPageLayout->addStretch();
        mainTab->addTab(defaultsPage,"Defaults");
        //
    }
    {
        QWidget* internetlPage = new QWidget(this);
        QVBoxLayout* internetlPageLayout = new QVBoxLayout(internetlPage);
        internetlPageLayout->setContentsMargins(1,1,1,1);
        internetlPage->setLayout(internetlPageLayout);

        //
        autoUpdates = new QCheckBox("Automatically check for new versios. ", internetlPage);
        autoUpdates->setEnabled(false);
        internetlPageLayout->addWidget(autoUpdates);
        internetlPageLayout->addStretch();
        mainTab->addTab(internetlPage,"Updates");
        //
    }


    mainLayout->addWidget(mainTab);
    mainLayout->addStretch();
}

ViewSettings::ViewSettings(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1,1,1,1);
    this->setLayout(mainLayout);

    visualisationGroup = new QGroupBox("Visualisation Options", this);
    QGridLayout* visualisationGroupLayout = new QGridLayout(visualisationGroup);
    visualisationGroupLayout->setContentsMargins(1,1,1,1);
    visualisationGroup->setLayout(visualisationGroupLayout);
    //
    {
        colorMapCombo = new QComboBox(visualisationGroup);
        QLabel* lbl_colormap = new QLabel("Default colormap:", visualisationGroup);
        visualisationGroupLayout->addWidget(lbl_colormap,0,0);
        colorMapCombo->addItems(display::ColorMap::getColormapList());
        visualisationGroupLayout->addWidget(colorMapCombo,0,1);
    }

    {
        QLabel* lbl_preview =new QLabel("Preview:", visualisationGroup);
        visualisationGroupLayout->addWidget(lbl_preview,1,0);

        int size = 80;
        QVector<QVector<double> > vtest1(size);
        for (int i = 0; i < size; ++i)
        {
            vtest1[i].resize(size);
            for(int j = 0; j < size; ++j)
                vtest1[i][j] = sqrt(i*i + j*j);
        }

        preview = new Display_container_2d(2, visualisationGroup);
        preview->set_display(vtest1);
        preview->enable_axis(false);
        visualisationGroupLayout->addWidget(preview,1,1);
        connect(colorMapCombo,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                preview, &Display_container_2d::set_color_map);
    }

    visualisationGroup->setLayout(visualisationGroupLayout);
    mainLayout->addWidget(visualisationGroup);
}

AppearanceSettings::AppearanceSettings(QWidget *parent) :
    QWidget(parent)
{

}

Savvy_settings::Savvy_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Savvy_settings)
{
    ui->setupUi(this);
    setWindowTitle("Preferences");

    general_setts = new GeneralSettings(this);
    ui->verticalLayout_2->addWidget(general_setts);

    view_setts = new ViewSettings(this);
    ui->verticalLayout_2->addWidget(view_setts);

    appearance_setts = new AppearanceSettings(this);
    ui->verticalLayout_2->addWidget(appearance_setts);

    ui->verticalLayout_2->addStretch();

    general_setts->setHidden(true);
    view_setts->setHidden(true);

    QSettings settings;
    view_setts->colorMapCombo->setCurrentIndex(settings.value("defaultColorMap").toInt());
    general_setts->autoContrast->setChecked(settings.value("AutoScaleImages").toBool());
    general_setts->pluginsPath->setText(settings.value("PluginsPath").toString());
    general_setts->autoPlotOpenedFiles->setChecked(settings.value("AutoPlotOpenedImages").toBool());
    general_setts->defaultAxis->setChecked(settings.value("showAxisDefault").toBool());
    general_setts->defaultTabbedMode->setChecked(settings.value("defaultTabbedViewMode").toBool());

    ui->listWidget->setCurrentRow(0);
    connect(ui->pb_save, &QPushButton::clicked, this, &Savvy_settings::on_save_settings);
}

Savvy_settings::~Savvy_settings()
{
    delete ui;
}

void Savvy_settings::on_listWidget_currentRowChanged(int currentRow)
{
    general_setts->setHidden(true);
    view_setts->setHidden(true);
    appearance_setts->setHidden(true);

    switch (currentRow) {
    case 0:
        general_setts->setHidden(false);
        break;
    case 1:
        view_setts->setHidden(false);
        break;
    case 2:
        appearance_setts->setHidden(false);
        break;
    default:
        break;
    }
}

void Savvy_settings::on_save_settings()
{
    QSettings settings;
    settings.setValue("defaultColorMap", view_setts->colorMapCombo->currentIndex());
    settings.setValue("AutoScaleImages", general_setts->autoContrast->isChecked());
    settings.setValue("PluginsPath", general_setts->pluginsPath->text());
    settings.setValue("AutoPlotOpenedImages", general_setts->autoPlotOpenedFiles->isChecked());
    settings.setValue("showAxisDefault", general_setts->defaultAxis->isChecked());
    settings.setValue("defaultTabbedViewMode", general_setts->defaultTabbedMode->isChecked());

    this->close();
}
