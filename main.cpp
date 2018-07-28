#include "src/gui/savvywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("University_Of_Hull");
    QCoreApplication::setOrganizationDomain("hull.ac.uk");
    QCoreApplication::setApplicationName("SAvVy");

    SavvyWindow w;
    w.show();

    return a.exec();
}
