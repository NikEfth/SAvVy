#include "src/gui/savvywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QCoreApplication::setOrganizationName("University_Of_Hull");
    QCoreApplication::setOrganizationDomain("hull.ac.uk");
    QCoreApplication::setApplicationName("SAvVy");

    SavvyWindow savvy_window;
    savvy_window.show();

    return application.exec();
}
