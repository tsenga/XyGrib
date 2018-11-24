#include "POI_TableWidget.h"

#include <iostream>

POI_TableWidget::POI_TableWidget(QWidget* parent,
                                 POI_Manager *poiManager,
                                 GriddedPlotter *plotter)
    : MeteoWidget(parent, plotter)
{
    this->poiManager = poiManager;
    this->plotter = plotter;

    std::cout << "before createTable" << std::flush;

    createTable();
}
//-------------------------------------------------------------------------------
bool POI_TableWidget::showWindArrows()
{
    return Util::getSetting("POI_showWindArrows", true).toBool();
}
//-------------------------------------------------------------------------------
bool POI_TableWidget::showCurrentArrows()
{
    return Util::getSetting("POI_showCurrentArrows", true).toBool();
}

void POI_TableWidget::createTable()
{
    this->reader = plotter->getReader();

    if (!reader) return;

    std::cout << "createTable" << std::flush;

    std::set<time_t> sdates = reader->getListDates();
    std::set<time_t>::iterator iter;
    std::set<time_t>::iterator iter2;

    int lig, col, colspan;
    QString dstr;

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

    addCell_title_dataline("POI Name", true, lig, col);

    col++;

    for (iter=sdates.begin(); iter!=sdates.end(); iter++) {
        time_t date = *iter;

        addCell_title(Util::formatTime(date), false, layout, lig, col, 1, 1, dateEpoch==date);

        col++;

        lsdates.push_back(date); // record date details to iterate through later
    }

    // content...

    DataCode windDTC(GRB_PRV_WIND_XY2D,LV_ABOV_GND,10);

    QMap<uint, POI*>::iterator it = poiManager->begin();

    while (it != poiManager->end()) {
        POI *poi = it.value();

        lig++;

        addCell_title_dataline(poi->getName(), true, lig, 0 /* col */);

        addLine_Wind(windDTC.getAltitude(), poi->getLongitude(), poi->getLatitude(), lig);

        ++it;
    }
}
//-----------------------------------------------------------------
void POI_TableWidget::addLine_Wind (const Altitude &alt, double lon, double lat, int lig)
{
    std::vector <time_t>::iterator iter;
    QColor    bgColor = Qt::white;
    QString   txt;
    int col = 0;

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
                if ( Util::getSetting("POI_showWindBeauforts", true).toBool() ) {
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

