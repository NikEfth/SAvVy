#ifndef NOTSOSIMPLE_GUITESTS_H
#define NOTSOSIMPLE_GUITESTS_H

#include "guitests.h"

class NotSoSimple_GuiTests : public GuiTests
{
    Q_OBJECT
public:
    explicit NotSoSimple_GuiTests(QObject *parent = nullptr);

    virtual bool run_tests();
signals:

public slots:
};

#endif // NOTSOSIMPLE_GUITESTS_H
