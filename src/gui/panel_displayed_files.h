#ifndef PANEL_DISPLAYED_FILES_H
#define PANEL_DISPLAYED_FILES_H

#include <QWidget>
#include <QListWidgetItem>

#include <memory>

#include "src/display/display_container.h"
#include "src/display/display_manager.h"
#include "stir/Array.h"
#include "stir/VectorWithOffset.h"


namespace Ui {
class Panel_displayed_files;
}

//!
//! \brief The Panel_opened_files class
//! This QWidget holds the panel of the QListWidget with the
//! names of the files openned in the QMdi.
class Panel_displayed_files : public QWidget
{
    Q_OBJECT

public:
    explicit Panel_displayed_files(QWidget *parent = nullptr);
    ~Panel_displayed_files();
    //! Get the row of the Display_container that has _id
    qint16 findQListWidgetItem( const QString& _id);
    //! Highlight the entry with _id
    void set_active(int _id);
    //! Change the name of  the item in ui->ListOpenedFiles
    void rename(const QString& _id, QString _new_name);
    //! Change the name of  the item in ui->ListOpenedFiles
    void rename(const int& _id, QString _new_name);
    //!
    bool has_grouped_items() const ;

    QVector<QString> get_groupped() const ;

signals:
    void double_clicked_item(QString);

public slots:
    //! Append a window on the  ui->listOpenedFiles
    void appendToOpenedList(Display_container* child);
    //! Append a window on the  ui->listOpenedFiles
    void appendToOpenedList(Display_manager* child);

    //! Remove item from the ui->listOpenedFiles list.
    void removeFromOpenedList(DisplayInterface *child);

private slots:
    void on_listOpenedFiles_itemDoubleClicked(QListWidgetItem *item);
    //!
    //! \brief highlightChecked
    //! \param item
    //! This function highlights listed items that have been grouped.
    //! In addition, it performs the grouping, by appending the new
    //! object to the grouped list.
    void highlightChecked(QListWidgetItem *item);

private:
    Ui::Panel_displayed_files *ui;

    //! Groupped items, saved by their unique ids.
    QVector<QString> grouped_windows;

    std::vector< std::shared_ptr<stir::ArrayInterface> > openned_files;
};

#endif // PANEL_OPENED_FILES_H
