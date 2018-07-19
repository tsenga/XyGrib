#include "POI_TableWidget.h"
#include "MeteoTableWidget.h"

POI_TableWidget::POI_TableWidget(QWidget* parent,
                                 GriddedPlotter *plotter,
                                 QList<POI*> listPOI)
    : QWidget(parent)
{
    this->parent = parent;
    this->plotter = plotter;
    this->listPOI = listPOI;

    this->layout = new QGridLayout(this);
    assert(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    headerWidget = new QWidget();
    assert(headerWidget);

    headerLayout = new QGridLayout(headerWidget);
    assert(headerLayout);

    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->setSpacing(0);

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

    //addCell_title_dataline("", true, 0 /* lig */, col);

    if (showSunMoonAlmanac) {
        //addCell_SunMoonAlmanac(daterecord, lat, lon, layout, 1 /* lig */, col, 1, colspan);
        lig++;
    }

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

            //addCell_title(curr, true, layout, 0 /* lig */, col, 1, colspan);

            if (showSunMoonAlmanac) {
                //addCell_SunMoonAlmanac (daterecord, lat, lon, layout, 1 /* lig */,col, 1,colspan);
            }

            col += colspan; // get start column for next date ready
        }
    }

    // Section 2 - one column for date + hours

    //DataPointInfo *pinfo;
    time_t dateEpoch = reader->getClosestDateFromNow();
    col = 0;
    lig++;

    // addCell_title_dataline("", true, lig, col);

    col++;


    for (iter=sdates.begin(); iter!=sdates.end(); iter++) {
        time_t date = *iter;

        // addCell_title(Util::formatTime(date), false, layout, lig, col, 1, 1, dateEpoch==date);
        col++;

        // Grib data for this point and this date
        //pinfo = new DataPointInfo(reader, lon, lat, date);
        //lspinfos.push_back(pinfo);
        //lsdates.push_back(date);
    }

    // content...
}

