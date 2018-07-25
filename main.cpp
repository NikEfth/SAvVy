#include "src/gui/savvywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    SavvyWindow savvy_window;
    savvy_window.show();

    return application.exec();
}
