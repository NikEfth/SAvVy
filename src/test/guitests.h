#ifndef GUITESTS_H
#define GUITESTS_H

#include <QObject>

class GuiTests : public QObject
{
    Q_OBJECT
public:
    explicit GuiTests(QObject *parent = nullptr);

signals:

public slots:

public:
    virtual bool run_tests() = 0;

protected:
    //! Ask a question to the user
    int ask(QString);
};

#endif // GUITESTS_H
