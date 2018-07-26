#ifndef Workspace_H
#define Workspace_H

#include <QWidget>
#include <QListWidgetItem>

#include <memory>

#include "stir/Array.h"
#include "stir/VectorWithOffset.h"


namespace Ui {
class Workspace;
}

//!
//! \brief The Workspace class
//! The Workspace holds all stir::Arrays loaded in the memory. In a sense  tries to
//! imetate the Matlab Workspace.
//! \todo This should move in the IO
//! \todo names should be unique
class Workspace : public QWidget
{
    Q_OBJECT

public:
    explicit Workspace(QWidget *parent = nullptr);
    ~Workspace();
    //! Change the name of  the item in ui->ListOpenedFiles
    void rename(const QString& _id, QString _new_name);
    //! Change the name of  the item in ui->ListOpenedFiles
    void rename(const int& _id, QString _new_name);
    //!
    bool has_grouped_items() const ;

    /** \addtogroup Getters
     *  @{
     */
    //! Get the list of groupped files
    QStringList get_groupped() const ;
    //! Get the name of the current seleted item at ui->listOpenedFiles
    QString get_current_name();
    //! Get the name of  the name of the item at ui->listOpenedFiles on row _i
    QString get_array_name(int _i);
    //! Get a pointer to an array with index _i
    std::shared_ptr<stir::ArrayInterface> get_array_ptr(int _i);

    std::shared_ptr<stir::ArrayInterface> get_array_ptr(const QString& _s);

    std::shared_ptr<stir::ArrayInterface> get_current_array_ptr();
    /** @}*/

    //! Create a stir::Array with the specified range and append the _name to
    //! ui->listOpenedFiles.
    template<int T>
    std::shared_ptr<stir::ArrayInterface> create_array_ptr(const stir::IndexRange<T> &, const QString& _name);

    std::shared_ptr<stir::ArrayInterface> open_array(const QString& fileName);

signals:
    void double_clicked_item(QString);
    //! Signal emitted when the ui->display_array is pressed.This is connected to
    //! SavvyWindow and triggers the create of a new MDI window.
    void display_current(std::shared_ptr<stir::ArrayInterface>, QString);

public slots:
    //! Append a window on the  ui->listOpenedFiles
    void append_to_workspace(std::shared_ptr<stir::ArrayInterface> child, const QString &_name);
    //! Update the state of the QWidgets on the GUI
    void updateGUI();

private slots:
    //    void on_listOpenedFiles_itemDoubleClicked(QListWidgetItem *item);
    //!
    //! \brief highlightChecked
    //! \param item
    //! This function highlights listed items that have been grouped.
    //! In addition, it performs the grouping, by appending the new
    //! object to the grouped list.
    void highlightChecked(QListWidgetItem *item);

    void on_remove_array_pressed();

    void on_display_array_pressed();

    void on_duplicate_array_clicked();

private:

    //! \details This member holds an incremental number for  every window that
    //! opens in the life time of the current run. It can only go upwards, starting from
    //! 1.
    qint16 next_dataset_id;
    //! Create a new record in ui->listOpenedFiles with the specified name
//    void append_to_openedFiles(const QString&);
    //! Remove from listOpenedFiles the item at row _id
    void remove_from_workspace(int _id);

    Ui::Workspace *ui;
    //! All the openned files are held by this vector
    QVector< std::shared_ptr<stir::ArrayInterface> > openned_files;
};



template<int T>
std::shared_ptr<stir::ArrayInterface> Workspace::create_array_ptr(const stir::IndexRange<T> &range, const QString& _name)
{

    std::shared_ptr<stir::ArrayInterface> ret(new stir::Array<T, float>(range));

    append_to_workspace(ret, _name);

    return openned_files[openned_files.size()-1];
}

#endif // PANEL_OPENED_FILES_H
