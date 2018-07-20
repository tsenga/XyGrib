#ifndef POI_TABLEWIDGET_H
#define POI_TABLEWIDGET_H

#include "MeteoWidget.h"
#include "POI.h"

class POI_TableWidget : public MeteoWidget
{
    Q_OBJECT
public:
    explicit POI_TableWidget(QWidget *parent,
                             GriddedPlotter* plotter,
                             QList<POI*> listPOI);
    void refresh();
    void refresh(GriddedPlotter* plotter);

signals:

public slots:

private:
        QList<POI*>         listPOI;
        std::vector<time_t> lsdates;

        void createTable();

        void addLine_Wind(const Altitude &alt, double lon, double lat, int lig);
};

#endif // POI_TABLEWIDGET_H
