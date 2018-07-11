#include "pluginsdialog.h"
#include "ui_pluginsdialog.h"
#include "external_interface.h"

#include <QPluginLoader>
#include <QStringList>
#include <QDir>

PlugInsDialog::PlugInsDialog(const QString &path, const QStringList &fileNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlugInsDialog)
{
    ui->setupUi(this);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                            QIcon::Normal, QIcon::On);
    interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                            QIcon::Normal, QIcon::Off);
    featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

    setWindowTitle(tr("Plugin Information"));
    findPlugins(path, fileNames);
}

PlugInsDialog::~PlugInsDialog()
{
    delete ui;
}

void PlugInsDialog::findPlugins(const QString &path,
                               const QStringList &fileNames)
{
    ui->label->setText(tr("Plug & Paint found the following plugins\n"
                      "(looked in %1):")
                   .arg(QDir::toNativeSeparators(path)));

    const QDir dir(path);

    foreach (QObject *plugin, QPluginLoader::staticInstances())
        populateTreeWidget(plugin, tr("%1 (Static Plugin)")
                                   .arg(plugin->metaObject()->className()));

    foreach (QString fileName, fileNames) {
        QPluginLoader loader(dir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
            populateTreeWidget(plugin, fileName);
    }
}

void PlugInsDialog::populateTreeWidget(QObject *plugin, const QString &text)
{
    QTreeWidgetItem *pluginItem = new QTreeWidgetItem(ui->treeWidget);
    pluginItem->setText(0, text);
    ui->treeWidget->setItemExpanded(pluginItem, true);

    QFont boldFont = pluginItem->font(0);
    boldFont.setBold(true);
    pluginItem->setFont(0, boldFont);

    if (plugin) {
        ExternalInterface *iProc = qobject_cast<ExternalInterface *>(plugin);
        if (iProc)
            addItems(pluginItem, "ExternalInterface", iProc->get_name());
    }
}

void PlugInsDialog::addItems(QTreeWidgetItem *pluginItem,
                            const char *interfaceName,
                            const QString &name)
{
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
    interfaceItem->setText(0, interfaceName);
    interfaceItem->setIcon(0, interfaceIcon);

        QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
        featureItem->setText(0, name);
        featureItem->setIcon(0, featureIcon);
}
