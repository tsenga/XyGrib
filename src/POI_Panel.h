#ifndef POI_PANEL_H
#define POI_PANEL_H

#include <QDockWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>

#include "GriddedPlotter.h"

#include "POI_TableWidget.h"
#include "POI_Manager.h"

class POI_Panel : public QDockWidget
{ Q_OBJECT
public:
    POI_Panel(POI_Manager *poiManager, QWidget *parent);

    void setPlotter(GriddedPlotter *plotter);

public slots:
    void slotPOICreated(POI*);
    void slotPOIDeleted(POI*);
    void slotPOIChanged(POI*);

private:
    POI_Manager         *poiManager;
    GriddedPlotter      *plotter;

    POI_TableWidget     *poiTable;

    QWidget             *createDataTable();

    QWidget             *dataTable;
 //   QVBoxLayout         *mainLayout;
    QScrollArea 		*scrollArea;
    QScrollArea         *hdrScroll;

};

#endif // POI_PANEL_H
