#ifndef POI_TABLEWIDGET_H
#define POI_TABLEWIDGET_H

#include "MeteoWidget.h"
#include "POI.h"
#include "POI_Manager.h"

class POI_TableWidget : public MeteoWidget
{
    Q_OBJECT
public:
    explicit POI_TableWidget(QWidget *parent,
                             POI_Manager *POI_Manager,
                             GriddedPlotter *plotter);

    void refresh();

signals:

public slots:

protected:
    virtual bool showWindArrows();
    virtual bool showCurrentArrows();


private:
        POI_Manager         *poiManager;
        std::vector<time_t> lsdates;

        void createTable();

        void addLine_Wind(const Altitude &alt, double lon, double lat, int lig);
};

#endif // POI_TABLEWIDGET_H
