#ifndef COMMON_DISPLAY_H
#define COMMON_DISPLAY_H

#include <QWidget>
#include <memory>

#include "display_container.h"

using namespace std;

namespace display {

class DisplayInterface : public QWidget
{
    Q_OBJECT

public:
    inline explicit DisplayInterface(int _id, int _dims, QWidget *parent = nullptr):
        QWidget(parent),
        dims(_dims),
        my_id(_id)
    {

    }

    ~DisplayInterface()
    {

    }

    /** \addtogroup Getters
     *  @{
     */
    inline int get_num_dimensions() const
    {
        return dims;
    }

    virtual size_t get_num_points() const = 0;

    virtual shared_ptr< QVector<double> >  get_x_values() const = 0;

    virtual shared_ptr< QVector<double> >  get_y_values() const = 0;

    inline QString get_file_name() const
    {
        return fullFileName;
    }

    inline shared_ptr<Display_container> get_display() const
    {
        return _display;
    }

    //! \return the unique (per session) id for this window
    inline int get_my_id() const
    {
        return my_id;
    }
    /** @}*/

    /** \addtogroup Setters
     *  @{
     */

    virtual void set_curve(const QVector<double> & _x_array,
                           const QVector<double> & _y_array,
                           bool replace = true, int after = 1,
                           bool symbols = false, bool line = false) = 0;

    virtual void set_no_controls(bool) = 0;

    /** @}*/

protected:

    shared_ptr<Display_container> _display = nullptr;
    //! File name and path
    QString fullFileName;
    //! Number of dimensions of the data
    int dims;
    //! This number is unique within a session
    int my_id;
};

}

#endif //COMMON_DISPLAY_H
