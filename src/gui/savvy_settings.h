#ifndef SAVVY_SETTINGS_H
#define SAVVY_SETTINGS_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>

#include "src/display/display_container_2d.h"

namespace Ui {
class Savvy_settings;
}

class GeneralSettings : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettings(QWidget *parent = nullptr);

     QCheckBox* autoPlotOpenedFiles;

    QCheckBox* autoContrast;

    QCheckBox* autoUpdates;
    //! \warning There is a small condrum with this option. We initialise
    //! our window before we set data into it. Therefore the size has not been
    //! determined, yet. Only the axis give a meaningfull size to the window. Therfore
    //! if they are deactivated, early from the contructor, the window will been drawn
    //! collapsed.
    QCheckBox* defaultAxis;

    QCheckBox* defaultTabbedMode;

    QVBoxLayout* mainLayout;

    QLineEdit* pluginsPath;
};


class ViewSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ViewSettings(QWidget *parent = nullptr);

    QVBoxLayout* mainLayout;

    QGroupBox* visualisationGroup;

    QComboBox* colorMapCombo;

    Display_container_2d* preview;
};

class AppearanceSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppearanceSettings(QWidget *parent = nullptr);


};


//!
//! \brief The Savvy_settings class
//! Class to read and write the default settings from the system.
//! It depends on QSettings.
class Savvy_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Savvy_settings(QWidget *parent = nullptr);
    ~Savvy_settings();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    //! Activated when ui->pb_save is pressed.
    void on_save_settings();
private:
    Ui::Savvy_settings *ui;
    //! General settings of the application
    GeneralSettings* general_setts;
    //! View related settings
    ViewSettings* view_setts;
    //! Settings of color for different visual elements
    AppearanceSettings* appearance_setts;
};

#endif // SAVVY_SETTINGS_H
