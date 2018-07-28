
#include <QtTest/QtTest>

#include "src/include/savvy.h"
#include "stir/Array.h"
#include "stir/Coordinate2D.h"

using namespace stir;

//!
//! \brief The Test_conversions class
//! A test class for conversions between number of dimentions
//! and supported arrays. The general concept is to test the functions
//! in \file savvy.h.
class Test_conversions: public QObject
{
    Q_OBJECT

public:

    void test_conversions_on_QVectors();

    void test_conversions_on_Arrays_and_QVectors();

};

void Test_conversions::test_conversions_on_QVectors()
{
//    int size = 30;
//    QVector<QVector<double> > vtest1(size);

//    for (int i = 0; i < size; ++i)
//        vtest1[i].resize(size);

//    for (int i = 0; i < size; ++i)
//        for (int j = 0; j < size; ++j)
//            vtest1[i][j] = sqrt(i*i + j*j);

//    QVector <double> vtest2;

//    savvy::serialize_QVector(vtest1, vtest2);

//    QVector<QVector<double> > vtest3;

//    savvy::deserialize_QVector(vtest2, vtest3);

//    for (int i = 0; i <= size; ++i)
//        for (int j = 0; j <= size; ++j)
//            QCOMPARE(vtest1[i][j], vtest3[i][j]);

}

void Test_conversions::test_conversions_on_Arrays_and_QVectors()
{
    //    int size = 30;

    //    const IndexRange<2> range(stir::Coordinate2D<int>(0, 0),
    //                              stir::Coordinate2D<int>(size,size));
    //    stir::Array<2, float> test1(range);

    //    for (int i = test1.get_min_index(); i <= test1.get_max_index() ; ++i)
    //        for (int j = test1[i].get_min_index(); j <= test1[i].get_max_index() ; ++j)
    //        {
    //            float f = sqrt(i*i + j*j);
    //            if( f != 0)
    //                test1[i][j] = sin(f) / f;
    //            else
    //                test1[i][j] = 1;
    //        }

    //    QVector<QVector<double> > vtest1(size);

    //    for (int i = 0; i < size; ++i)
    //        vtest1[i].resize(size);

    //    savvy::Array2QVector(test1, vtest1);

    //    for (int i = 0; i <= size; ++i)
    //        for (int j = 0; j <= size; ++j)
    //        QCOMPARE(vtest1[i][j], test1[i][j]);
}


QTEST_MAIN(Test_conversions)
#include "test_conversions.moc"
