#include "MeteoWidget.h"

#include "Astro.h"

MeteoWidget::MeteoWidget(GriddedPlotter* plotter, QWidget *parent) : QWidget(parent)
{
    this->plotter = plotter;
    this->parent = parent;

    layout = new QGridLayout(this);
    assert(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    headerWidget = new QWidget();
    assert(headerWidget);

    headerLayout = new QGridLayout(headerWidget);
    assert(headerLayout);

    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->setSpacing(0);
}
//------------------------------------------------------------------------
QWidget * MeteoWidget::getDataHeaders ()
{
    return headerWidget;
}

//----------------------------------------------------------------
void MeteoWidget::addCell_title_dataline (QString txt, bool bold,
                int lig,int col)
{
    QColor bgcolor(200,200,255);
    TableCell *cell2 = new TableCell (headerWidget, txt, bold, bgcolor,
                                        Qt::AlignRight|Qt::AlignHCenter);
    assert (cell2);
    cell2->setBorders (lig, col);
    headerLayout->addWidget (cell2, lig,col, 1,1);
}
//----------------------------------------------------------------
void MeteoWidget::addCell_SunMoonAlmanac (time_t t, double lat, double lon,
                QGridLayout *layout,int lig,int col, int rowspan,int colspan)
{
    TableCell *cell = new TableCell (this);
    assert (cell);
    cell->setBorders (lig, col);

    Almanac alm = Astro::getSunMoonAlmanac (t, lat, lon);

    QString rs,st;
    QString txt;
    rs = alm.sunRise>0 ? Util::formatTime (alm.sunRise) : "----";
    st = alm.sunSet>0 ? Util::formatTime (alm.sunSet) : "----";
    txt = "⇑ "+rs+" ⇓ "+st+"\n";
    rs = alm.moonRise>0 ? Util::formatTime (alm.moonRise) : "----";
    st = alm.moonSet>0 ? Util::formatTime (alm.moonSet) : "----";
    txt += "⇑ "+rs+" ⇓ "+st;
    txt += "  "+QString("%1%").arg( (int)(100*alm.moonDisk+0.5));
    cell->label->setText (txt);
    layout->setAlignment (Qt::AlignLeft);
    layout->addWidget (cell, lig,col, rowspan,colspan );
}
//----------------------------------------------------------------
void MeteoWidget::addCell_title (QString txt, bool bold,
                QGridLayout *layout,int lig,int col, int rowspan,int colspan,
                bool isNowDate)
{
    QColor bgcolor(200,200,255);
    if (isNowDate) {
        bgcolor = QColor(250,250,100);
    }
    TableCell *cell = new TableCell(this, txt, bold, bgcolor);
    assert (cell);
    cell->setBorders (lig, col);
    layout->addWidget (cell, lig,col, rowspan,colspan );
}
//----------------------------------------------------------------
void MeteoWidget::addCell_content (
                QString txt,
                QGridLayout *layout,
                bool south,
                int lig,int col,
                int    rowspan,
                int    colspan,
                QColor bgcolor,
                int    cellType,
                double  vx,
                double  vy
                )
{
    TableCell *cell;
    if (cellType == MTABLE_WIND_CELL) {
        cell = new TableCell_Wind (vx, vy, south, plotter,
                    this, txt, false, bgcolor);
    }
    else if (cellType == MTABLE_CURRENT_CELL) {
        cell = new TableCell_Current (vx, vy, south, plotter,
                    this, txt, false, bgcolor);
    }
    else if (cellType == MTABLE_CLOUD_CELL) {
        cell = new TableCell_Clouds (vx, plotter,
                    this, txt, false, bgcolor);
    }
    else {
        cell = new TableCell (this, txt, false, bgcolor);
    }
    assert (cell);
    cell->setBorders (lig, col);
    layout->addWidget (cell, lig,col, rowspan,colspan );
}
//===================================================================
void MeteoWidget::addCell_SkewT (
                QGridLayout *layout,double lon, double lat, QString locationName, int lig,int col,
                time_t date
                )
{
    TableCell *cell = new TableCell_SkewT (this, lon, lat, locationName, date, reader);
    assert (cell);
    cell->setBorders (lig, col);
    layout->addWidget (cell, lig,col, 1,1, Qt::AlignCenter);
}
//------------------------------------------------------------------------
void MeteoWidget::showEvent (QShowEvent */*event*/)
{
    QRect r;
    // Adjust header height to row height
    for (int j=0; j<layout->rowCount(); j++)
    {
        QLayoutItem *it1 = layout->itemAtPosition (j,1);
        if (it1 && it1->widget()) {
            QLayoutItem *it2 = headerLayout->itemAtPosition (j,0);
            if (it2 && it2->widget())
            {
                it2->widget()->setMinimumHeight (it1->widget()->height());
            }
        }
    }
}

//===================================================================
// TableCell : case seule
//===================================================================
TableCell::TableCell(QWidget *parent, QString txt, bool bold,
                        QColor bgcolor,
                        Qt::Alignment alignment
                    )
    : QWidget(parent)
{
    this->bgcolor = bgcolor;
    this->bordercolor = QColor(100,100,100);
    this->borders = TableCell::none;

    label = new QLabel (txt, this);
//	label->setAlignment (alignment);
    if (bold) {
        QFont font;
        font.setBold(true);
        label->setFont(font);
    }
    setContrastedTextColor(this->bgcolor);

    QGridLayout *layout = new QGridLayout (this);
    int mh=2, mv=4;
    layout->setContentsMargins (mh,mv,mh,mv);
//	layout->addWidget(label, 0,0, Qt::AlignHCenter|Qt::AlignBottom);
    layout->addWidget(label, 0,0, alignment);
}
//----------------------------------------------------------
void    TableCell::setContrastedTextColor(QColor bgcolor)
{
    QColor fgcolor = DataColors::getContrastedColor (bgcolor);
    QPalette p;
    p.setBrush(QPalette::Active, QPalette::WindowText, fgcolor);
    p.setBrush(QPalette::Inactive, QPalette::WindowText, fgcolor);
    label->setPalette(p);
}
//---------------------------------------------------------
void TableCell::setBorders (int lig, int col)
{
    if (lig==0 && col==0)
        this->setBorders (TableCell::all);
    else if (lig==0)
        this->setBorders (TableCell::south+TableCell::east+TableCell::north);
    else if (col==0)
        this->setBorders (TableCell::west+TableCell::south+TableCell::east);
    else
        this->setBorders (TableCell::south+TableCell::east);
}
//---------------------------------------------------------
void TableCell::paintEvent(QPaintEvent * /*event*/)
{
    QPainter pnt(this);

    pnt.fillRect(0,0,width(),height(), QBrush(bgcolor));

    QPen pen(bordercolor);
    pen.setWidth(1);
    pnt.setPen(pen);

    if (borders & TableCell::north)
        pnt.drawLine(0,0, width()-1,0);
    if (borders & TableCell::south)
        pnt.drawLine(0,height()-1, width()-1,height()-1);

    if (borders & TableCell::west)
        pnt.drawLine(0,0, 0,height()-1);
    if (borders & TableCell::east)
        pnt.drawLine(width()-1,0, width()-1,height()-1);

}


//===================================================================
// TableCell_Wind : case seule spécialisée pour le vent (flêche+barbules)
//===================================================================
TableCell_Wind::TableCell_Wind (double vx, double vy, bool south,
                    GriddedPlotter *plotter,
                    QWidget *parent, QString txt, bool bold,
                    QColor bgcolor )
    : TableCell(parent, txt, bold, bgcolor)
{
    this->vx = vx;
    this->vy = vy;
    this->south = south;
    this->plotter = plotter;

    windArrowsColor = QColor(40,40,40);
    showWindArrows = Util::getSetting("MTABLE_showWindArrows", true).toBool();

    if (showWindArrows && vx!=GRIB_NOTDEF && vy!=GRIB_NOTDEF)
        setMinimumHeight(label->minimumSizeHint().height()+50);
}
//---------------------------------------------------------
void TableCell_Wind::paintEvent(QPaintEvent * e)
{
    TableCell::paintEvent(e);
    QPainter pnt(this);
    pnt.setRenderHint(QPainter::Antialiasing, true);

    if (showWindArrows && vx != GRIB_NOTDEF && vx != GRIB_NOTDEF)
    {
        plotter->drawWindArrowWithBarbs(
                pnt, width()/2, 25, vx, vy, south, windArrowsColor);
    }
}


//===================================================================
// TableCell_Current : case seule spécialisée pour le courant (flêche)
//===================================================================
TableCell_Current::TableCell_Current (double cx, double cy, bool south,
                    GriddedPlotter *plotter,
                    QWidget *parent, QString txt, bool bold,
                    QColor bgcolor )
    : TableCell(parent, txt, bold, bgcolor)
{
    this->cx = cx;
    this->cy = cy;
    this->south = south;
    this->plotter = plotter;

    currentArrowsColor = QColor(40,40,40);
    showCurrentArrows = Util::getSetting("MTABLE_showCurrentArrows", true).toBool();

    if (showCurrentArrows && cx!=GRIB_NOTDEF && cy!=GRIB_NOTDEF)
        setMinimumHeight(label->minimumSizeHint().height()+50);
}
//---------------------------------------------------------
void TableCell_Current::paintEvent(QPaintEvent * e)
{
    TableCell::paintEvent(e);
    QPainter pnt(this);
    pnt.setRenderHint(QPainter::Antialiasing, true);

    if (showCurrentArrows && cx != GRIB_NOTDEF && cx != GRIB_NOTDEF)
    {
        plotter->drawCurrentArrow (
                pnt, width()/2, 25, cx, cy, south, currentArrowsColor);
    }
}


//===================================================================
// TableCell_Clouds : case seule spécialisée pour la nébulosité
//===================================================================
TableCell_Clouds::TableCell_Clouds (
                    double   val,
                    GriddedPlotter *plotter,
                    QWidget  *parent, QString txt, bool bold,
                    QColor bgcolor )
    : TableCell(parent, txt, bold, bgcolor)
{
    this->plotter = plotter;
    this->val = val;

    // Color = seaColor + bgColor
    QColor seaColor (50,50,200, 255);
    QImage img (1,1, QImage::Format_ARGB32_Premultiplied);
    QPainter pnt (&img);

    plotter->setCloudsColorMode("MTABLE_cloudsColorMode");
    QColor cloudColor = QColor::fromRgba(plotter->getCloudColor(val, true));
    plotter->setCloudsColorMode("cloudsColorMode");

    pnt.fillRect (0,0, 1,1, seaColor );
    pnt.fillRect (0,0, 1,1, cloudColor);
    this->bgcolor = QColor(img.pixel(0,0));

    setContrastedTextColor(this->bgcolor);
}
//---------------------------------------------------------
void TableCell_Clouds::paintEvent(QPaintEvent * e)
{
    TableCell::paintEvent(e);
    QPainter pnt(this);
    pnt.setRenderHint(QPainter::Antialiasing, true);

}



//===================================================================
// TableCell_SkewT
//===================================================================
TableCell_SkewT::TableCell_SkewT (QWidget *parent,
                    double lon, double lat,  QString locationName,
                    time_t date,
                    GriddedReader *reader)
    : TableCell (parent)
{
    this->lon = lon;
    this->lat = lat;
    this->locationName = locationName;
    this->date = date;
    this->reader = reader;
    MiniSkewT miniskewt;
    miniskewt.initFromGriddedReader (reader, lon, lat, date);
    pixmap = miniskewt.createPixmap ();
    setFixedSize (miniskewt.size());
}
//---------------------------------------------------------
void TableCell_SkewT::paintEvent (QPaintEvent * e)
{
    TableCell::paintEvent (e);
    QPainter pnt (this);
    pnt.setRenderHint(QPainter::Antialiasing, true);
    pnt.drawPixmap (0,0, pixmap);
}
//---------------------------------------------------------
void TableCell_SkewT::enterEvent (QEvent *)
{
    setCursor (Qt::PointingHandCursor);
}
//---------------------------------------------------------
void TableCell_SkewT::leaveEvent (QEvent *)
{
    unsetCursor ();
}
//---------------------------------------------------------
void TableCell_SkewT::mouseReleaseEvent (QMouseEvent *e)
{
    if (this->rect().contains (e->pos())) {
        SkewT *skewt = new SkewT ();
        skewt->initFromGriddedReader (reader, lon, lat, date);

        SkewTWindow *sdial = new SkewTWindow (skewt);

        QString position = Util::formatPosition(lon, lat);
        if (locationName == "") {
            skewt->setLocation (position);
            sdial->setWindowTitle ("SkewT - "+position);
        }
        else {
            skewt->setLocation (position+" ("+locationName+")");
            sdial->setWindowTitle ("SkewT - "+locationName);
        }
        sdial->show ();
    }
}
