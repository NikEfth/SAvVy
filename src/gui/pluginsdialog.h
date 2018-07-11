#ifndef PLUGINSDIALOG_H
#define PLUGINSDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QTreeWidget>

namespace Ui {
class PlugInsDialog;
}

class PlugInsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlugInsDialog(const QString &path, const QStringList &fileNames,
                           QWidget *parent);
    ~PlugInsDialog();

private:
    Ui::PlugInsDialog *ui;

    QIcon interfaceIcon;
    QIcon featureIcon;

    void findPlugins(const QString &path,
                                   const QStringList &fileNames);

    void populateTreeWidget(QObject *plugin, const QString &text);

    void addItems(QTreeWidgetItem *pluginItem,
                                const char *interfaceName,
                                const QString &name);
};

#endif // PLUGINSDIALOG_H
