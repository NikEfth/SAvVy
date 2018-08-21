#ifndef Workspace_H
#define Workspace_H

#include <QWidget>
#include <QListWidgetItem>
#include <QPoint>

#include <memory>

#include "stir/Array.h"
#include "stir/VectorWithOffset.h"


namespace Ui {
class Workspace;
}

//!
//! \brief The Workspace class
//! The Workspace holds all stir::Arrays loaded in the memory.
//! In a sense tries to immetate the Matlab Workspace.
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
    //! Get the index numbers of the groupped items. The function
    //! returns the number of groupped items.
    unsigned int get_groupped(QVector<int> *index_list = nullptr) const ;

    unsigned long int get_size_of_data_in_group() const;

    unsigned long int get_num_of_data_in_group() const;

    unsigned long int get_num_of_openned_files() const;

    int get_min_max(double& min, double& max,
                    const int i = 0,
                    const int min_pos = -1, const int pos_range = -1) const;
    //! Get the name of the current seleted item at ui->listOpenedFiles
    QString get_current_name() const;
    //! Get the name of  the name of the item at ui->listOpenedFiles on row _i
    QString get_array_name(int _i) const;
    //! Get a pointer to an array with index _i
    std::shared_ptr<stir::ArrayInterface> get_array_ptr(const int _i) const;

    std::shared_ptr<stir::ArrayInterface> get_array_ptr(const QString& _s) const;

    std::shared_ptr<stir::ArrayInterface> get_current_array_ptr();

    std::shared_ptr<stir::ArrayInterface> get_new_empty_copy(const QString name = "", const int i = 0);

    void go_to_top() const;
    //! Get the next groupped item. If not existing or end of list then \return
    //! -1 otherwise \return the current index.
    int get_next_item_in_group(std::shared_ptr<stir::ArrayInterface> &ret) const;
    //! As get_next_item_in_group but in addition perfoms a serialisation of the
    //! array. min_slice and slice_range can be used to reduce the amount of data returned.
    //! The operation will take place as min_pos + pos_range.
    int get_next_item_in_group_as_vector(std::shared_ptr<QVector<double> >& ret, const int min_pos = -1, const int pos_range = -1);

    bool check_all_grouped_have_same_characteristics();
    /** @}*/

    //! Create a stir::Array with the specified range and append the _name to
    //! ui->listOpenedFiles.
    template<int T>
    std::shared_ptr<stir::ArrayInterface> create_array_ptr(const stir::IndexRange<T> &, const QString& _name);

    std::shared_ptr<stir::ArrayInterface> open_array(const QString& fileName);

    bool write_file_to_disk(const QString fileName, int pos = 0) const;

    bool write_current_file_to_disk(const QString fileName) const;

signals:
    void double_clicked_item(QString);
    //! Signal emitted when the ui->display_array is pressed.This is connected to
    //! SavvyWindow and triggers the create of a new MDI window.
    void display_current(std::shared_ptr<stir::ArrayInterface>, QString);

public slots:
    //! Append a window on the  ui->listOpenedFiles
    int append_to_workspace(std::shared_ptr<stir::ArrayInterface> data, const QString &name);

    void append_to_workspace(std::shared_ptr<QVector<QVector<QVector<double> > > > data,
                                        const QString& name);
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
    void highlightChecked(QListWidgetItem *item) const;

    void on_remove_array_pressed();

    void on_display_array_pressed();

    void on_duplicate_array_clicked();

    void on_psh_move_up_clicked();

    void on_psh_move_down_clicked();

    void on_psh_info_clicked();

    void showListContextMenu(const QPoint &pos);

private:

    //! \details This member holds an incremental number for  every window that
    //! opens in the life time of the current run. It can only go upwards, starting from
    //! 1.
    qint16 next_dataset_id;
    //! Create a new record in ui->listOpenedFiles with the specified name
    //    void append_to_openedFiles(const QString&);
    //! Remove from listOpenedFiles the item at row _id
    void remove_from_workspace(const int _id);

    Ui::Workspace *ui;
    //! All the openned files are held by this vector
    QVector< std::shared_ptr<stir::ArrayInterface> > openned_files;

    bool hadData = false;
};



template<int T>
std::shared_ptr<stir::ArrayInterface> Workspace::create_array_ptr(const stir::IndexRange<T> &range, const QString& _name)
{

    std::shared_ptr<stir::ArrayInterface> ret(new stir::Array<T, float>(range));

    append_to_workspace(ret, _name);

    return openned_files[openned_files.size()-1];
}

#endif // PANEL_OPENED_FILES_H
