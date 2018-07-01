#include "src/gui/savvywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SavvyWindow w;
    w.show();

    return a.exec();
}
