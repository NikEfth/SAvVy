#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QWidget>
#include "src/display/common_display.h"

#include "stir/is_null_ptr.h"

using namespace display;

class WatchDog : public QWidget
{
    Q_OBJECT
public:
    explicit WatchDog(QWidget *parent = nullptr);

    virtual ~WatchDog();
    virtual void setContainer(DisplayInterface * _s) = 0;
    //! Unset the current Screen_manager.
    virtual void unsetContainer() = 0;

signals:
    void container_set();

    void container_unset();

public slots:

protected:
    DisplayInterface* m_current_container_ptr = nullptr;


};

#endif // WATCHDOG_H
