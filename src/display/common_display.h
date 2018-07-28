#ifndef COMMON_DISPLAY_H
#define COMMON_DISPLAY_H

#include <qwt_color_map.h>
#include <QWidget>
#include <memory>

namespace display {

class DisplayInterface : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayInterface(int _id, int _dims, QWidget *parent = nullptr)
        : QWidget(parent),
          dims(_dims),
          my_id(_id)
    {    }

    ~DisplayInterface() {}

    /** \addtogroup Getters
     *  @{
     */
    inline int get_num_dimensions() const
    {
        return dims;
    }

    virtual size_t get_num_points() const = 0;

    virtual std::shared_ptr< QVector<double> >  get_x_values() const = 0;

    virtual std::shared_ptr< QVector<double> >  get_y_values() const = 0;

    inline QString get_file_name() const
    {
        return fullFileName;
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

    //! File name and path
    QString fullFileName;
    //! Number of dimensions of the data
    int dims;
    //! This number is unique within a session
    int my_id;
};

//! Colormaps. Defaults at BW grayscale.
class ColorMap: public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap()
    {
        set_BW();
        Nstops = 256;
        my_list = ColorMap::getColormapList();
    }

    ColorMap(int _set):
        QwtLinearColorMap()
    {
        set_BW();
        Nstops = 256;
        my_list = ColorMap::getColormapList();

        setColormap(_set);
    }

    ~ColorMap()
    {

    }
    //! Get the list with all the available options
    static QStringList getColormapList()
    {
        QStringList ret;
        ret<<"BW"<<"WB"<<"Viridis" <<"Extended Black Body"<<"QWT"<<"Jet";
        return ret;
    }
    //! Set the active ColorMap by name
    inline void setColormap(QString name)
    {
        switch (my_list.indexOf(name)) {
        case 0:
            this->set_BW();
            break;
        case 1:
            this->set_WB();
            break;
        case 2:
            this->set_Viridis();
            break;
        case 3:
            this->set_EBB();
            break;
        case 4:
            this->set_qwt();
            break;
        case 5:
            this->set_JET();
            break;
        default:
            this->set_BW();
            break;
        }
    }
    //! Set the active ColorMap by index
    inline void setColormap(int index)
    {
        switch (index) {
        case 0:
            this->set_BW();
            break;
        case 1:
            this->set_WB();
            break;
        case 2:
            this->set_Viridis();
            break;
        case 3:
            this->set_EBB();
            break;
        case 4:
            this->set_qwt();
            break;
        case 5:
            this->set_JET();
            break;
        default:
            this->set_BW();
            break;
        }
    }
    //! Set Black - White grayscale
    inline void set_BW()
    {
        background = Qt::black;
        peak = Qt::yellow;
        setColorInterval(Qt::black, Qt::white);
    }
    //! Set White - Black grayscale
    inline void set_WB()
    {
        background = Qt::white;
        peak = Qt::black;
        setColorInterval(Qt::white, Qt::black);
    }
    //! Set Jet, popular by old Matlab
    inline void set_JET()
    {
        background = Qt::blue;
        setColorInterval(Qt::blue, Qt::red);
        peak = Qt::white;
        addColorStop( 0.35, Qt::cyan );
        addColorStop( 0.5, Qt::green );
        addColorStop( 0.75, Qt::yellow );
    }
    //! Set QWT default ColorMap
    inline void set_qwt()
    {
        background = Qt::darkCyan;
        setColorInterval(Qt::darkCyan, Qt::red);
        peak = Qt::yellow;
        addColorStop( 0.1, Qt::cyan );
        addColorStop( 0.6, Qt::green );
        addColorStop( 0.95, Qt::yellow );
    }
    //! Set Viridis ColorMap
    inline void set_Viridis()
    {
        double step = 1.0 / Nstops;
        double cur = 0.0;

        for (int i = 0; i < Nstops; i++, cur+=step)
        {
            _data[i] = QColor(_viridis_data[i][0],
                    _viridis_data[i][1],
                    _viridis_data[i][2]);

            addColorStop(cur,_data[i]);
        }

        background = _data[0];
        peak = _data[Nstops-1];
        setColorInterval(_data[0], _data[Nstops-1]);
    }
    //! Set Extended Black Body ColorMap
    inline void set_EBB()
    {
        double step = 1.0 / Nstops;
        double cur = 0.0;

        for (int i = 0; i < Nstops; i++, cur+=step)
        {
            _data[i] = QColor(_ebb_data[i][0],
                    _ebb_data[i][1],
                    _ebb_data[i][2]);

            addColorStop(cur,_data[i]);
        }

        background = _data[0];
        peak = _data[Nstops-1];
        setColorInterval(_data[0], _data[Nstops-1]);
    }
    //! Returns the first Color of the ColorMap
    inline QColor get_background() const
    { return background; }
    //! Returns the last Color of the ColorMap
    inline QColor get_peak_color() const
    { return peak; }

private:

    QColor _data[256];
    //! Vidiris array. I could find the functions.
    quint8 _viridis_data[256][3] = {
        68, 1,  84,
        68, 2,  85,
        68, 3,  87,
        69, 5,  88,
        69, 6,  90,
        69, 8,  91,
        70, 9,  92,
        70, 11, 94,
        70, 12, 95,
        70, 14, 97,
        71, 15, 98,
        71, 17, 99,
        71, 18, 101,
        71, 20, 102,
        71, 21, 103,
        71, 22, 105,
        71, 24, 106,
        72, 25, 107,
        72, 26, 108,
        72, 28, 110,
        72, 29, 111,
        72, 30, 112,
        72, 32, 113,
        72, 33, 114,
        72, 34, 115,
        72, 35, 116,
        71, 37, 117,
        71, 38, 118,
        71, 39, 119,
        71, 40, 120,
        71, 42, 121,
        71, 43, 122,
        71, 44, 123,
        70, 45, 124,
        70, 47, 124,
        70, 48, 125,
        70, 49, 126,
        69, 50, 127,
        69, 52, 127,
        69, 53, 128,
        69, 54, 129,
        68, 55, 129,
        68, 57, 130,
        67, 58, 131,
        67, 59, 131,
        67, 60, 132,
        66, 61, 132,
        66, 62, 133,
        66, 64, 133,
        65, 65, 134,
        65, 66, 134,
        64, 67, 135,
        64, 68, 135,
        63, 69, 135,
        63, 71, 136,
        62, 72, 136,
        62, 73, 137,
        61, 74, 137,
        61, 75, 137,
        61, 76, 137,
        60, 77, 138,
        60, 78, 138,
        59, 80, 138,
        59, 81, 138,
        58, 82, 139,
        58, 83, 139,
        57, 84, 139,
        57, 85, 139,
        56, 86, 139,
        56, 87, 140,
        55, 88, 140,
        55, 89, 140,
        54, 90, 140,
        54, 91, 140,
        53, 92, 140,
        53, 93, 140,
        52, 94, 141,
        52, 95, 141,
        51, 96, 141,
        51, 97, 141,
        50, 98, 141,
        50, 99, 141,
        49, 100,141,
        49, 101,141,
        49, 102,141,
        48, 103,141,
        48, 104,141,
        47, 105,141,
        47, 106,141,
        46, 107,142,
        46, 108,142,
        46, 109,142,
        45, 110,142,
        45, 111,142,
        44, 112,142,
        44, 113,142,
        44, 114,142,
        43, 115,142,
        43, 116,142,
        42, 117,142,
        42, 118,142,
        42, 119,142,
        41, 120,142,
        41, 121,142,
        40, 122,142,
        40, 122,142,
        40, 123,142,
        39, 124,142,
        39, 125,142,
        39, 126,142,
        38, 127,142,
        38, 128,142,
        38, 129,142,
        37, 130,142,
        37, 131,141,
        36, 132,141,
        36, 133,141,
        36, 134,141,
        35, 135,141,
        35, 136,141,
        35, 137,141,
        34, 137,141,
        34, 138,141,
        34, 139,141,
        33, 140,141,
        33, 141,140,
        33, 142,140,
        32, 143,140,
        32, 144,140,
        32, 145,140,
        31, 146,140,
        31, 147,139,
        31, 148,139,
        31, 149,139,
        31, 150,139,
        30, 151,138,
        30, 152,138,
        30, 153,138,
        30, 153,138,
        30, 154,137,
        30, 155,137,
        30, 156,137,
        30, 157,136,
        30, 158,136,
        30, 159,136,
        30, 160,135,
        31, 161,135,
        31, 162,134,
        31, 163,134,
        32, 164,133,
        32, 165,133,
        33, 166,133,
        33, 167,132,
        34, 167,132,
        35, 168,131,
        35, 169,130,
        36, 170,130,
        37, 171,129,
        38, 172,129,
        39, 173,128,
        40, 174,127,
        41, 175,127,
        42, 176,126,
        43, 177,125,
        44, 177,125,
        46, 178,124,
        47, 179,123,
        48, 180,122,
        50, 181,122,
        51, 182,121,
        53, 183,120,
        54, 184,119,
        56, 185,118,
        57, 185,118,
        59, 186,117,
        61, 187,116,
        62, 188,115,
        64, 189,114,
        66, 190,113,
        68, 190,112,
        69, 191,111,
        71, 192,110,
        73, 193,109,
        75, 194,108,
        77, 194,107,
        79, 195,105,
        81, 196,104,
        83, 197,103,
        85, 198,102,
        87, 198,101,
        89, 199,100,
        91, 200,98,
        94, 201,97,
        96, 201,96,
        98, 202,95,
        100,203,93,
        103,204,92,
        105,204,91,
        107,205,89,
        109,206,88,
        112,206,86,
        114,207,85,
        116,208,84,
        119,208,82,
        121,209,81,
        124,210,79,
        126,210,78,
        129,211,76,
        131,211,75,
        134,212,73,
        136,213,71,
        139,213,70,
        141,214,68,
        144,214,67,
        146,215,65,
        149,215,63,
        151,216,62,
        154,216,60,
        157,217,58,
        159,217,56,
        162,218,55,
        165,218,53,
        167,219,51,
        170,219,50,
        173,220,48,
        175,220,46,
        178,221,44,
        181,221,43,
        183,221,41,
        186,222,39,
        189,222,38,
        191,223,36,
        194,223,34,
        197,223,33,
        199,224,31,
        202,224,30,
        205,224,29,
        207,225,28,
        210,225,27,
        212,225,26,
        215,226,25,
        218,226,24,
        220,226,24,
        223,227,24,
        225,227,24,
        228,227,24,
        231,228,25,
        233,228,25,
        236,228,26,
        238,229,27,
        241,229,28,
        243,229,30,
        246,230,31,
        248,230,33,
        250,230,34,
        253,231,36};

    quint8 _ebb_data[256][3] = {
        0,0,0,
        5,3,1,5,
        9,5,2,10,
        4,8,2,14,
        10,3,17,
        13,4,20,
        15,5,23,
        17,6,26,
        18,6,28,
        20,7,31,
        21,8,33,
        22,9,36,
        23,10,38,
        24,11,41,
        25,11,43,
        25,12,46,
        26,13,49,
        27,13,51,
        28,14,54,
        29,14,57,
        30,14,59,
        31,14,62,
        31,14,65,
        32,15,68,
        33,15,71,
        34,15,73,
        35,15,76,
        36,15,79,
        37,15,82,
        37,15,85,
        38,15,88,
        39,15,91,
        40,15,94,
        41,15,97,
        41,15,100,
        42,15,103,
        43,15,106,
        44,15,109,
        44,15,112,
        45,15,115,
        46,15,118,
        47,14,121,
        47,14,124,
        48,14,128,
        49,14,131,
        49,14,134,
        50,13,137,
        51,13,140,
        51,13,143,
        52,12,147,
        53,12,150,
        53,12,153,
        54,11,156,
        55,11,160,
        55,10,163,
        56,9,166,
        57,9,169,
        59,8,171,
        61,7,173,
        64,6,175,
        66,5,176,
        68,4,178,
        70,3,180,
        72,2,182,
        74,1,184,
        76,0,186,
        79,0,187,
        81,0,189,
        83,0,191,
        85,0,193,
        87,0,195,
        89,0,197,
        91,0,199,
        93,0,200,
        95,0,202,
        97,0,204,
        99,0,206,
        101,0,208,
        103,0,210,
        105,0,212,
        107,0,214,
        109,0,216,
        111,0,217,
        113,0,219,
        116,0,221,
        118,0,223,
        120,0,225,
        122,0,227,
        124,0,229,
        130,0,223,
        135,0,218,
        140,0,212,
        145,0,207,
        149,0,202,
        153,0,196,
        157,0,191,
        161,0,185,
        165,0,180,
        168,0,175,
        171,0,170,
        174,0,164,
        177,0,159,
        180,0,154,
        183,0,149,
        185,0,143,
        188,0,138,
        190,0,133,
        193,0,128,
        195,0,123,
        197,0,118,
        199,0,113,
        201,0,107,
        203,0,102,
        205,0,97,
        207,0,92,
        209,0,87,
        210,0,82,
        212,4,77,
        214,8,72,
        15,13,66,
        17,17,61,
        17,22,61,
        18,26,61,
        19,30,61,
        20,34,61,
        20,37,61,
        21,40,61,
        22,43,61,
        23,46,61,
        23,48,61,
        24,51,61,
        25,53,61,
        26,56,61,
        26,58,61,
        27,60,61,
        28,62,61,
        28,64,60,
        29,67,60,
        30,69,60,
        31,71,60,
        31,73,60,
        32,74,60,
        33,76,60,
        34,78,60,
        34,80,60,
        35,82,60,
        36,84,60,
        36,86,59,
        37,87,59,
        38,89,59,
        38,91,59,
        39,92,59,
        40,94,59,
        41,96,59,
        41,98,59,
        42,99,58,
        3,101,58,
        3,103,58,
        4,104,58,
        5,106,58,
        5,107,58,
        6,109,57,
        7,111,57,
        7,112,57,
        8,114,57,
        9,115,57,
        0,117,56,
        9,119,56,
        9,121,56,
        9,124,55,
        9,126,55,
        8,128,54,
        8,130,54,
        8,132,53,
        8,135,53,
        7,137,52,
        7,139,52,
        7,141,51,
        6,143,51,
        6,145,50,
        6,147,50,
        5,149,49,
        5,151,48,
        5,153,48,
        4,155,47,
        4,157,47,
        4,159,46,
        3,161,45,
        3,163,45,
        2,165,44,
        2,167,43,
        2,169,42,
        1,170,41,
        1,172,41,
        0,174,40,
        0,176,39,
        9,178,38,
        9,180,37,
        8,182,36,
        8,184,35,
        7,185,34,
        7,187,33,
        6,189,32,
        6,191,30,
        5,193,29,
        5,194,28,
        4,196,26,
        3,198,24,
        3,200,23,
        2,202,21,
        2,203,19,
        1,205,17,
        0,207,14,
        0,209,11,
        9,211,8,
        9,212,17,
        0,213,32,
        1,214,43,
        2,215,52,
        3,216,60,
        4,217,67,
        5,218,74,
        6,219,81,
        7,220,87,
        8,221,93,
        9,222,99,
        0,223,105,
        1,224,110,
        2,225,116,
        3,227,121,
        4,228,127,
        4,229,132,
        5,230,137,
        6,231,143,
        7,232,148,
        7,233,153,
        8,234,158,
        9,235,163,
        9,236,168,
        0,237,173,
        0,238,179,
        1,239,184,
        1,241,189,
        2,242,194,
        2,243,199,
        3,244,204,
        3,245,209,
        3,246,214,
        4,247,219,
        4,248,224,
        4,249,229,
        4,251,235,
        5,252,240,
        5,253,245,
        5,254,250};

    QColor background;
    QColor peak;

    QStringList my_list;
    //! Number of color stops \todo Adjustable
    int Nstops;
};

}
#endif
