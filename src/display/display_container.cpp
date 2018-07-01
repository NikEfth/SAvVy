#include "display_container.h"

void Display_container::closeEvent(QCloseEvent *event)
{
    emit aboutToClose();
}
