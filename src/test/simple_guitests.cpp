//#include "simple_guitests.h"
//#include "stir/common.h"
//#include "stir/IndexRange.h"
//#include "stir/Array.h"

//#include <memory>
//using namespace stir;

//Simple_GuiTests::Simple_GuiTests(QObject *parent) : GuiTests(parent)
//{

//}

//bool Simple_GuiTests::run_tests()
//{
//    bool all_tests = true;

//    all_tests = test_display_1d_data();

//    //all_tests = ask(QString("Is this the output of a sinc function? "));

//    all_tests = test_display_1d_data_physical();

//    //all_tests = ask(QString("Has the range of x axis reduced half? "));

//    all_tests = test_display_2d_data();

//    //all_tests = ask(QString("Does the image look like a ripple? "));

//    all_tests = test_display_2d_data_alt();

//    //all_tests = ask(QString("Was this the same as the previous? "));

//    all_tests = test_display_2d_data_physical();

//    //all_tests = ask(QString("Has the range of the image reduced half? "));

//    all_tests = test_display_2d_data_alt_not_square();

//    //all_tests = ask(QString("Was this the same as the previous? "));

//    all_tests = test_display_2d_data_physical_not_square();

//    //all_tests = ask(QString("Has the range of the image reduced half? "));

//    all_tests = test_display_3d_data();
//}

//bool Simple_GuiTests::test_display_1d_data()
//{

//    const IndexRange<1> range(-60,60);
//    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->create_array_ptr<1>(range, "test1");
//    stir::Array<1, float>* test1 = dynamic_cast<stir::Array<1, float>* >(test1_sptr.get());

//    for (int i = test1->get_min_index(); i <= test1->get_max_index() ; ++i)
//    {
//        if(i != 0)
//        {
//            float f = static_cast<float>(i);
//            test1->at(i) = sin(f) / f;
//        }
//        else
//            test1->at(i) = 1;
//    }

//    QString n = pnl_workspace->get_current_name() + "_indiced";

//    display_array(test1_sptr, n);

//    return true;
//}

//bool Simple_GuiTests::test_display_1d_data_physical()
//{

//    std::shared_ptr <stir::ArrayInterface> test1_sptr = pnl_workspace->get_array_ptr(0);
//    QString n = pnl_workspace->get_current_name() + "_physical";

//    display_array(test1_sptr, n);

//    return true;
//}

//bool Simple_GuiTests::test_display_2d_data()
//{

//    const IndexRange<2> range(stir::Coordinate2D<int>(-60,-60),stir::Coordinate2D<int>(59,59));
//    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->create_array_ptr<2>(range, "test2");
//    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());


//    for (int i = test2->get_min_index(); i <= test2->get_max_index() ; ++i)
//        for (int j = (*test2)[i].get_min_index(); j <= (*test2)[i].get_max_index() ; ++j)
//        {
//            float f = sqrt(i*i + j*j);
//            if( f != 0)
//                (*test2)[i][j]  = sin(f) / f;
//            else
//                (*test2)[i][j] = 1;
//        }

//    QString n = pnl_workspace->get_current_name() + "_indiced";
//    display_array(test2_sptr, n);

//    return true;
//}

//bool Simple_GuiTests::test_display_2d_data_alt()
//{

//    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
//    QString n = pnl_workspace->get_current_name() + "_no_offset";
//    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());

//    int size = test2->size();
//    QVector<QVector<double> > vtest2(size);

//    for (int i = 0; i < size; ++i)
//        vtest2[i].resize(size);

//    savvy::Array2QVector(*test2, vtest2);

//    QVector<double> svtest2;
//    savvy::serialize_QVector(vtest2, svtest2);

//    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    child->set_file_name(n);

//    if (is_null_ptr(child))
//        return false;

//    child->set_display(svtest2, size);

//    append_to_mdi(child);

//    return true;
//}

//bool Simple_GuiTests::test_display_2d_data_physical()
//{

//    std::shared_ptr <stir::ArrayInterface> test2_sptr = pnl_workspace->get_current_array_ptr();
//    QString n = pnl_workspace->get_current_name() + "_physical";
//    stir::Array<2, float>* test2 = dynamic_cast<stir::Array<2, float>* >(test2_sptr.get());


//    int size = test2->size();
//    QVector<QVector<double> > vtest2(size);

//    for (int i = 0; i < size; ++i)
//        vtest2[i].resize(size);

//    savvy::Array2QVector(*test2, vtest2);

//    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    child->set_file_name(n);

//    if (is_null_ptr(child))
//        return false;

//    child->set_physical_display(vtest2,
//                                (*test2)[0].get_min_index(), (*test2).get_min_index(),
//            0.5, 0.5);

//    append_to_mdi(child);

//    return true;
//}

//bool Simple_GuiTests::test_display_2d_data_alt_not_square()
//{

//    int row_size = 60;
//    int row_num = 120;

//    int start_x= -row_size/2;
//    int start_y = -row_num/2;
//    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
//                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));

//    std::shared_ptr <stir::ArrayInterface> test3_sptr = pnl_workspace->create_array_ptr<2>(range, "test3");
//    stir::Array<2, float>* test3 = dynamic_cast<stir::Array<2, float>* >(test3_sptr.get());

//    for (int i = test3->get_min_index(); i <= test3->get_max_index() ; ++i)
//        for (int j = (*test3)[i].get_min_index(); j <= (*test3)[i].get_max_index() ; ++j)
//            (*test3)[i][j] = j;

//    int num_row = test3->size();
//    int size_row = (*test3)[0].size();
//    QVector<QVector<double> > vtest3(num_row);

//    for (int i = 0; i < num_row; ++i)
//        vtest3[i].resize(size_row);

//    savvy::Array2QVector(*test3, vtest3);

//    QVector<double> svtest3;
//    savvy::serialize_QVector(vtest3, svtest3);

//    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    QString n = pnl_workspace->get_current_name() + "_vertical";
//    child->set_file_name(n);

//    if (is_null_ptr(child))
//        return false;

//    child->set_display(svtest3, size_row,
//                       (*test3)[0].get_min_index(), (*test3).get_min_index());

//    append_to_mdi(child);

//    return true;
//}

//bool Simple_GuiTests::test_display_2d_data_physical_not_square()
//{
//    int row_size = 120;
//    int row_num = 60;

//    int start_x= -row_size/2;
//    int start_y = -row_num/2;

//    const IndexRange<2> range(stir::Coordinate2D<int>(start_x, start_y),
//                              stir::Coordinate2D<int>( start_x+row_size, start_y + row_num));
//    std::shared_ptr <stir::ArrayInterface> test4_sptr = pnl_workspace->create_array_ptr<2>(range, "test4");
//    stir::Array<2, float>* test4 = dynamic_cast<stir::Array<2, float>* >(test4_sptr.get());

//    for (int i = test4->get_min_index(); i <= test4->get_max_index() ; ++i)
//        for (int j = (*test4)[i].get_min_index(); j <= (*test4)[i].get_max_index() ; ++j)
//        {
//            float f = i;
//            (*test4)[i][j] = f;
//        }

//    QVector<QVector<double> > vtest4(test4->size());

//    for (int i = 0; i < test4->size(); ++i)
//        vtest4[i].resize((*test4)[0].size());

//    savvy::Array2QVector(*test4, vtest4);

//    Display_container_2d *child = dynamic_cast<Display_container_2d *>(createDisplayContainer(2));
//    QString n = pnl_workspace->get_current_name() + "_horizontal";
//    child->set_file_name(n);

//    if (is_null_ptr(child))
//        return false;

//    child->set_physical_display(vtest4,
//                                (*test4)[0].get_min_index(), (*test4).get_min_index(),
//            0.5, 0.5);

//    append_to_mdi(child);

//    return true;
//}

//bool Simple_GuiTests::test_display_3d_data()
//{

//    const IndexRange<3> range(stir::Coordinate3D<int>(-60,-60, -60),stir::Coordinate3D<int>(59,59, 59));
//    std::shared_ptr <stir::ArrayInterface> test5_sptr = pnl_workspace->create_array_ptr<3>(range, "test5");
//    stir::Array<3, float>* test5 = dynamic_cast<stir::Array<3, float>* >(test5_sptr.get());


//    for (int i = test5->get_min_index(); i <= test5->get_max_index() ; ++i)
//            for (int j = (*test5)[i].get_min_index(); j <= (*test5)[i].get_max_index() ; ++j)
//                for (int k = (*test5)[i][j].get_min_index(); k <= (*test5)[i][j].get_max_index() ; ++k)
//            {
//                float f = sqrt(i*i + j*j + k*k);
//                if( f != 0)
//                    (*test5)[i][j][k] = sin(f) / f;
//                else
//                    (*test5)[i][j][k] = 1;
//            }

//    QString n = pnl_workspace->get_current_name() + "_indiced";
//    display_array(test5_sptr, n);

//    return true;
//}
