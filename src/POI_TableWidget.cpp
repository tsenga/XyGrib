#include "POI_TableWidget.h"

POI_TableWidget::POI_TableWidget(QWidget* parent,
                                 GriddedPlotter *plotter,
                                 QList<POI*> listPOI)
    : MeteoWidget(plotter, parent)
{
    this->listPOI = listPOI;

    createTable();
}

void POI_TableWidget::createTable()
{
    this->reader = plotter->getReader();

    if (!reader) return;

    std::set<time_t> sdates = reader->getListDates();
    std::set<time_t>::iterator iter;
    std::set<time_t>::iterator iter2;

    int lig, col, colspan;
    QString dstr;

    bool showSunMoonAlmanac = Util::getSetting("MTABLE_showSunMoonAlmanac", true).toBool();

    // Section 1 - one top level column per day, columns for hours with days

    col = 0;
    lig = 0;

    addCell_title_dataline("", true, 0 /* lig */, col);

    col ++;
    QString curr = "";

    for (iter=sdates.begin(); iter!= sdates.end(); iter++) {
        time_t daterecord = *iter;
        dstr = Util::formatDateLong(daterecord);

        if (dstr != curr) { // if the date is different
            colspan = 0;
            curr = dstr;
            iter2 = iter;

            do {
                colspan++;
                iter2++;
                dstr = Util::formatDateLong(*iter2);
            } while (curr == dstr); // count out the sub-day buckets

            // set title, incorporating number of intra-day buckets

            addCell_title(curr, true, layout, 0 /* lig */, col, 1, colspan);

            col += colspan; // get start column for next date ready
        }
    }

    // Section 2 - one column for date + hours

    //DataPointInfo *pinfo;
    time_t dateEpoch = reader->getClosestDateFromNow();
    col = 0;
    lig++;

    addCell_title_dataline("", true, lig, col);

    col++;

    for (iter=sdates.begin(); iter!=sdates.end(); iter++) {
        time_t date = *iter;

        addCell_title(Util::formatTime(date), false, layout, lig, col, 1, 1, dateEpoch==date);

        col++;

        lsdates.push_back(date); // record date details to iterate through later
    }

    // content...

    DataCode windDTC(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10);

    QList<POI*>::iterator it;

    for (it=listPOI.begin(); it != listPOI.end(); it++) {
        POI *poi = *it;

        lig++;

        addLine_Wind(windDTC.getAltitude(), poi->getLongitude(), poi->getLatitude(), lig);
    }
}
//-----------------------------------------------------------------
void POI_TableWidget::addLine_Wind (const Altitude &alt, double lon, double lat, int lig)
{
    std::vector <time_t>::iterator iter;
    QColor    bgColor = Qt::white;
    QString   txt;
    int col = 0;
    addCell_title_dataline (tr("Wind")+" ("+AltitudeStr::toStringShort(alt)+")",
                    layout, lig,col);
    col ++;
    for (iter=lsdates.begin(); iter!=lsdates.end(); iter++, col++) {
        time_t curr_time = *iter;

        DataPointInfo pf(reader, lon, lat, curr_time);

        float v, dir;
        txt = "";
        bgColor = Qt::white;
        if (pf.getWindValues (alt, &v, &dir)) {
            if (dir != GRIB_NOTDEF) {
                QString tmp;
                tmp.sprintf("%.0f", dir);
                txt += tmp + tr(" °") + "\n";
                txt += Util::formatSpeed_Wind(v);
                if ( Util::getSetting("MTABLE_showWindBeauforts", true).toBool() ) {
                    tmp.sprintf("%2d", Util::msToBeaufort(v));
                    txt += "\n";
                    txt += tmp + tr(" Bf");
                }
                bgColor = QColor(plotter->getWindColor(v, true));
            }
        }
        float vx, vy;
        pf.getWindVxVy (alt, &vx, &vy);
        addCell_content (txt, layout,(lat<0),lig,col, 1,1, bgColor, MTABLE_WIND_CELL,vx,vy);
    }
}

void POI_TableWidget::refresh()
{
}

void POI_TableWidget::refresh(GriddedPlotter *plotter)
{
    this->plotter = plotter;
}
