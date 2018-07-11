#include <QtTest/QtTest>

#include "src/display/display_container_1d.h"
#include "src/display/display_container_2d.h"
#include "stir/Coordinate2D.h"

#include "src/IO/io_manager.h"
#include "src/include/savvy.h"

using namespace stir;

class Test_display_container: public QObject
{
    Q_OBJECT

public:

private slots:

    void check_pass_1d();

    void check_pass_2d();

private:

};


void Test_display_container::check_pass_1d()
{

    Display_container_1d* foo = new Display_container_1d(0);

    const stir::IndexRange<1> range(-30,30);
    stir::Array<1, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        test1[i] = i;

    QVector<double> vtest1(test1.size());

   savvy::Array2QVector(test1, vtest1);

    // Pass by reference.
    foo->set_display(vtest1);

//    for (int i = test1.get_min_index(); i <= test1.get_max_index(); ++i)
//        QCOMPARE(foo->at(i), i);

    foo->clear();

    QVector<double> * p_test1 = &vtest1;
    // Pass by pointer.
    foo->set_display(p_test1);

//    for (int i = test1.get_min_index(); i <= test1.get_max_index(); ++i)
//        QCOMPARE(foo->at(i), i);

}

void Test_display_container::check_pass_2d()
{

    Display_container_2d *foo = new Display_container_2d(0);

    const IndexRange<2> range(stir::Coordinate2D<int>(-100,-100),stir::Coordinate2D<int>(99,99));
    stir::Array<2, float> test1(range);

    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
            test1[i][j] = i*100 - j*50;

    int size = test1.size();
    QVector<QVector<double> > vtest1(size);

    for (int i = 0; i < size; ++i)
        vtest1[i].resize(size);

    savvy::Array2QVector(test1, vtest1);

    foo->set_display(vtest1);
    foo->set_sizes(test1.get_min_index(), test1[0].get_min_index());

//    for (int i =foo->get_min_index_v(); i < foo->get_max_index_v() ; ++i)
//        for (int j = foo->get_min_index_h(); j < foo->get_max_index_h() ; ++j)
//            QCOMPARE(foo->at(i , j), (i*100 - j*50));

}

QTEST_MAIN(Test_display_container)
#include "test_display_container.moc"
