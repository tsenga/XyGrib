#ifndef METEOWIDGET_H
#define METEOWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "GriddedPlotter.h"
#include "SkewT.h"
#include "DataPointInfo.h"

#define MTABLE_WIND_CELL    1
#define MTABLE_CLOUD_CELL   2
#define MTABLE_CURRENT_CELL 3
#define MTABLE_SKEWT_CELL   4

class MeteoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeteoWidget(QWidget *parent = nullptr, GriddedPlotter* plotter = nullptr);

    void setPlotter(GriddedPlotter *plotter);
    QWidget *getDataHeaders ();

signals:

public slots:

protected:
    GriddedPlotter* plotter;
    GriddedReader*  reader;

    QWidget*        parent;

    QGridLayout*    layout;
    QWidget*        headerWidget;
    QGridLayout*    headerLayout;

    virtual bool showWindArrows() = 0;
    virtual bool showCurrentArrows() = 0;

    void addCell_title (QString txt, bool bold,
                    QGridLayout *layout,int lig,int col,
                    int rowspan=1, int colspan=1,
                    bool isNowDate=false);

    void addCell_SunMoonAlmanac (time_t t, double lat, double lon,
                    QGridLayout *layout,int lig,int col, int rowspan,int colspan);

    void addCell_title_dataline (QString txt, bool bold,
                    int lig,int col);


    void addCell_content (QString txt,
                    QGridLayout *layout,
                    bool south,
                    int lig,int col,
                    int    rowspan=1,
                    int    colspan=1,
                    QColor bgcolor=Qt::white,
                    int    cellType = 0,
                    double  vx=0,
                    double  vy=0
                    );

    void addCell_SkewT (QGridLayout *layout,double lon, double lat, QString locationName, int lig,int col, time_t date);

    void showEvent (QShowEvent *event);
};

//===================================================================
class TableCell : public QWidget
{ Q_OBJECT
    public:
        TableCell (QWidget *parent, QString txt="", bool bold=false,
                                        QColor bgcolor=QColor(200,200,255),
                                        Qt::Alignment alignment=Qt::AlignHCenter|Qt::AlignBottom
                        );

                void setBorders (int lig, int col);
            QLabel  *label;

    protected:
            void  	paintEvent(QPaintEvent *event);
            void    setContrastedTextColor(QColor bgColor);

                static const uint none  = 0;
                static const uint north = 1;
                static const uint west  = 2;
                static const uint south = 4;
                static const uint east  = 8;
                static const uint all   = 1+2+4+8;

                void setBorders (int num) { borders=num; }

            QColor  bgcolor, bordercolor;
            int     borders;
};


//===================================================================
class TableCell_SkewT : public TableCell
{ Q_OBJECT
    public:
        TableCell_SkewT (QWidget *parent,
                                                 double lon, double lat, QString locationName,
                                                 time_t date,
                                                 GriddedReader *reader);
    protected:
                QPixmap pixmap;
                double lon, lat;
                QString locationName;
                time_t date;
                GriddedReader *reader;
            void  paintEvent (QPaintEvent *event);
                void  enterEvent (QEvent *);
                void  leaveEvent (QEvent *);
                void  mouseReleaseEvent (QMouseEvent *);
};

//===================================================================
class TableCell_Wind : public TableCell
{ Q_OBJECT
    public:
        TableCell_Wind(
                                double vx, double vy, bool south,
                                GriddedPlotter  *plotter,
                                QWidget *parent, QString txt, bool bold,
<<<<<<< HEAD
                                QColor bgcolor, bool showWindArrows
=======
                                QColor bgcolor
>>>>>>> b53e3ef3f91713e1400b2019df214daf1a0c4c6a
                                );
    protected:
        QColor    windArrowsColor;
        double 	  vx, vy;
        bool	  south;
        bool	  showWindArrows;
        GriddedPlotter *plotter;
            void  	paintEvent(QPaintEvent *event);
};

//===================================================================
class TableCell_Current : public TableCell
{ Q_OBJECT
    public:
        TableCell_Current(
                                double cx, double cy, bool south,
                                GriddedPlotter  *plotter,
                                QWidget *parent, QString txt, bool bold,
<<<<<<< HEAD
                                QColor bgcolor, bool showCurrentArrows
=======
                                QColor bgcolor
>>>>>>> b53e3ef3f91713e1400b2019df214daf1a0c4c6a
                                );
    protected:
        QColor    windArrowsColor;
        QColor    currentArrowsColor;
        double 	  cx, cy;
        bool	  south;
        bool	  showCurrentArrows;
        GriddedPlotter *plotter;
            void  	paintEvent(QPaintEvent *event);
};

//===================================================================
class TableCell_Clouds : public TableCell
{ Q_OBJECT
    public:
        TableCell_Clouds (
                                double val,
                                GriddedPlotter *plotter,
                                QWidget *parent, QString txt, bool bold,
                                QColor bgcolor
                                );
    protected:
        GriddedPlotter *plotter;
        double   val;
            void  	 paintEvent(QPaintEvent *event);
};


#endif // METEOWIDGET_H
