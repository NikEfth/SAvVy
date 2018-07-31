#ifndef COMMON_DISPLAY_H
#define COMMON_DISPLAY_H

#include <qwt_color_map.h>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QSharedPointer>
#include <memory>
#include <array>

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
class ColorMap
{
public:
    inline explicit ColorMap(QString plugin_colormaps = "",
                             QString default_colormap = ""):
        _plugin_colormaps(plugin_colormaps),
        _default_colormap(default_colormap)
    {
        set_BW();
        set_WB();
        set_qwt();
        set_JET();

        my_list << "BW" << "WB" << "QWT" << "Jet";

        if (!_plugin_colormaps.isEmpty())
            load_plugins();

        if(!_default_colormap.isEmpty())
        {
            index_of_default_colormap = my_list.indexOf(_default_colormap);
        }

    }

    inline explicit ColorMap(int set,
                             QString plugin_colormaps = "",
                             QString default_colormap = ""):
        ColorMap(plugin_colormaps, default_colormap)
    {
        getColorMap(set);
    }

    inline ~ColorMap()
    {

    }

    //! Get the list with all the available options
    inline QStringList getColormapList()
    {
        return my_list;
    }

    //! Set the active ColorMap by name
    inline const QSharedPointer<QwtLinearColorMap> getColorMap(QString name) const
    {
        int index = my_list.indexOf(name);

        return getColorMap(index);
    }

    inline const QSharedPointer<QwtLinearColorMap> getDefaultColorMap() const
    {
        return get_ColorMap(index_of_default_colormap);
    }

    //! Set the active ColorMap by index
    inline const QSharedPointer<QwtLinearColorMap> getColorMap(int index) const
    {
        switch (index)
        {
        case 0:

            return this->get_BW();

        case 1:

            return this->get_WB();

        case 2:

            return this->get_qwt();

        case 3:

            return this->get_JET();

        default:

            return this->get_ColorMap(index);
        }

    }

    inline int load_plugins()
    {

        QStringList filter;
        QDirIterator it(_plugin_colormaps, filter,
                        QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while (it.hasNext())
        {
            QFile file(it.next());
            if (!file.exists())
                continue;
            QFileInfo info(file);

            if(info.suffix() != "csv")
                continue;

            QTextStream file_text_stream(&file);

            if(file.open(QIODevice::ReadOnly))
            {
                QSharedPointer<QwtLinearColorMap> tmp( new QwtLinearColorMap());

                QColor firstC;
                QColor lastC;

                int ind = 0;

                while(!file_text_stream.atEnd())
                {
                    QString line = file_text_stream.readLine();
                    QStringList split = line.split(',');

                    if (ind == 1)
                    {
                        firstC.setRgb(split[1].toInt(), split[2].toInt(), split[3].toInt());
                    }

                    if (ind == 256)
                    {
                        lastC.setRgb(split[1].toInt(), split[2].toInt(), split[3].toInt());
                    }

                    ++ind;
                }

                file.close();
                tmp->setColorInterval(firstC, lastC);

                file.open(QIODevice::ReadOnly);
                ind = 0;
                while(!file_text_stream.atEnd())
                {
                    QString line = file_text_stream.readLine();
                    QStringList split = line.split(',');

                    if(ind > 0 && ind < 256)
                    {
                        QColor clr(static_cast<unsigned char>(split[1].toInt()),
                                static_cast<unsigned char>(split[2].toInt()),
                                static_cast<unsigned char>(split[3].toInt()));

                        tmp->addColorStop(split[0].toDouble(), clr);
                    }
                    ++ind;
                }

                file.close();

                my_list << info.fileName();
                _data_pool.append(tmp);
            }
        }
        return 1;
    }

    //! Set Black - White grayscale
    inline void set_BW()
    {
        QSharedPointer<QwtLinearColorMap> tmp( new QwtLinearColorMap(Qt::black,
                                                                     Qt::white));

        _data_pool.append(tmp);
    }

    inline const QSharedPointer<QwtLinearColorMap> get_BW() const
    {
        return _data_pool[0];
    }

    //! Set White - Black grayscale
    inline void set_WB()
    {
        QSharedPointer<QwtLinearColorMap> tmp( new QwtLinearColorMap(Qt::white,
                                                                     Qt::black));

        _data_pool.append(tmp);
    }
    inline const QSharedPointer<QwtLinearColorMap> get_WB() const
    {
        return _data_pool[1];
    }
    //! Set Jet, popular by old Matlab
    inline void set_JET()
    {
        QSharedPointer<QwtLinearColorMap> tmp( new QwtLinearColorMap(Qt::blue,
                                                                     Qt::red));

        tmp->addColorStop( 0.35, Qt::cyan );
        tmp->addColorStop( 0.5, Qt::green );
        tmp->addColorStop( 0.75, Qt::yellow );

        _data_pool.append(tmp);
    }

    inline const QSharedPointer<QwtLinearColorMap> get_JET() const
    {
        return _data_pool.at(3);
    }
    //! Set QWT default ColorMap
    inline void set_qwt()
    {
        QSharedPointer<QwtLinearColorMap> tmp( new QwtLinearColorMap(Qt::darkCyan,
                                                                     Qt::red));

        tmp->addColorStop( 0.1, Qt::cyan );
        tmp->addColorStop( 0.6, Qt::green );
        tmp->addColorStop( 0.95, Qt::yellow );

        _data_pool.append(tmp);
    }

    inline QSharedPointer<QwtLinearColorMap> get_qwt() const
    {
        return _data_pool.at(2);
    }

    //! Set Extended Black Body ColorMap
    inline QSharedPointer<QwtLinearColorMap> get_ColorMap(int index) const
    {
        if (index < _data_pool.size() &&
                index >=0)
        {
            return _data_pool.at(index);
        }

        return nullptr;
    }

    //    //! Returns the first Color of the ColorMap
    //    inline QColor get_background() const
    //    { return background; }
    //    //! Returns the last Color of the ColorMap
    //    inline QColor get_peak_color() const
    //    { return peak; }

private:

    QString _plugin_colormaps;

    QString _default_colormap;

    int index_of_default_colormap;

    QVector < QSharedPointer<QwtLinearColorMap> > _data_pool;

    //    QColor background;
    //    QColor peak;

    QStringList my_list;
};
}

#endif //COMMON_DISPLAY_H
