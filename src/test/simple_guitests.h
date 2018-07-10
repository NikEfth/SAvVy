#ifndef SIMPLE_GUITESTS_H
#define SIMPLE_GUITESTS_H

#include "guitests.h"

class Simple_GuiTests : public GuiTests
{
    Q_OBJECT
public:
    explicit Simple_GuiTests(QObject *parent = nullptr);

    virtual bool run_tests();

private:
    /** \addtogroup Testing
     *  @{
     */

    //! This function will create a sinc plot. The plot will have 121 points with
    //! offset -60. The x axis is on indeces.
    //! - This test function will write the test array in Workspace
    bool test_display_1d_data();
    //! Similar to test_display_1d_data() but the array has 121 points, with
    //! sampling distance 0.5 (mm) therefore the boundaties are [-30,30).
    //! The x axis is float representing a physical sampling.
    //! - The array data are drawn from Workspace.
    bool test_display_1d_data_physical();
    //! Creates a 2D sinc plot in a matrix [200, 200], from (-100, -100) to (99, 99).
    //! This is the simplest case of 2D plotting handled by the application.
    //! - In this case a stir::Array<2> is created and the values are passed in a
    //! 1D QVector, from savvy::Array2QVector(const stir::Array<2, float> & input,
    //! QVector<QVector<double> > & output) and the 2D matrix is input to the
    //! Display_container_2d.
    //! - The result should look like a sereis of concetric circles with gradially
    //! reduced intensity. The circles should be centered around point (0,0).
    //! - The serialisation is done in the Display_container_2d. This approch should have
    //! a small speed advnatage as, in the same time the search for mininum and
    //! maximum value is performed.
    bool test_display_2d_data();
    //! This test function is similar to the test_display_2d_data(). The same
    //! simulation function, is used. The differences are
    //! # zero offset is used, therefore point (0,0) should be at the lower bottem
    //! corner
    //! # The savvy::serialize_QVector is called before the display function, therefore
    //! the 1D array is the input to Display_container_2d::set_display().
    bool test_display_2d_data_alt();
    //! A test for displaying non square matrices. This test is similar to
    //! test_display_2d_data_alt() but the size of the x axis is smaller than the size of
    //! y axis. The dipslayed gradient should move from left to right.
    //! The serialisation is performed calling the savvy::serialize_QVector()
    bool test_display_2d_data_alt_not_square();
    //! Similar to test_display_2d_data() but the physical sampling is every 0.5 (mm)
    //! therefore the size of the image boundaries are (-50, -50) to (49,49) mm . The
    //! serialisation is performed by calling savvy::serialize_QVector()
    bool test_display_2d_data_physical();
    //! Tests the display with physical sizes and non square form. In this case the
    //! Y axis is shorter and the gradient should move from bottom to top.
    bool test_display_2d_data_physical_not_square();

        bool test_display_3d_data();
    /** @}*/
};

#endif // SIMPLE_GUITESTS_H
