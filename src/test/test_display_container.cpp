#include <QtTest/QtTest>

#include "src/display/display_container_1d.h"
#include "src/display/display_container_2d.h"
#include "stir/Coordinate2D.h"

#include "src/IO/io_manager.h"

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

    Display_container_1d* foo = new Display_container_1d();

    const IndexRange<1> range(-30,30);
    stir::Array<1, float> test1(range);

    for (int i = -30; i <= 30 ; ++i)
        test1[i] = i;

    QVector<double> vtest1(test1.size());

    io_manager::Array2QVector(test1, vtest1);

    // Pass by reference.
    foo->set_array(vtest1, -30);

    for (int i = -30; i <= 30; ++i)
        QCOMPARE(foo->at(i), i);

    foo->empty();

    QVector<double> * p_test1 = &vtest1;

    foo->set_array(p_test1, -30);

    for (int i = -30; i <= 30; ++i)
        QCOMPARE(foo->at(i), i);

}

void Test_display_container::check_pass_2d()
{
    //    Display_container_1d* foo = new Display_container_1d();

    //    //    const IndexRange<2> range(Coordinate2D<int>(0,0),Coordinate2D<int>(9,9));
    //    //    stir::Array<2, float> test1(range);

    //    const IndexRange<1> range(0, 200);
    //    stir::Array<1, float> test1(range);

    //    test1.fill(0.0);
    //    for (int i = 0; i <= 200; ++i)
    //        test1[i] = 100 - i ;

    //    foo->set_array(test1);
    //    for (int i = 0; i <= 200; ++i)
    //        QCOMPARE(foo->at(i), 100-i);

    //    Display_container_2d* foo2 = new Display_container_2d();

    //    const IndexRange<2> range2(Coordinate2D<int>(0,0),Coordinate2D<int>(200,200));
    //    stir::Array<2, float> test2(range2);

    //    test2.fill(0.0);
    //    for (int i = 0; i <= 200; ++i)
    //        for (int j = 0; j <= 200; ++j)
    //        {
    //            test2[i][j] = 200*200 - i*j;
    //        }

    //    foo2->set_array(test2);
    //    for (int i = 0; i <= 200; ++i)
    //        for (int j = 0; j <= 200; ++j)
    //            QCOMPARE(foo2->at(i,j), 200*200 - i*j);

}

QTEST_MAIN(Test_display_container)
#include "test_display_container.moc"
