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

class POI_Panel : public QDockWidget
{ Q_OBJECT
public:
    POI_Panel(QWidget *parent);

    void refresh(GriddedPlotter* plotter);

public slots:
    void slotPOICreated(POI*);
    void slotPOIDeleted(POI*);

private:
    POI_TableWidget*    dataTable;
    POI_TableWidget*    createDataTable();
    QVBoxLayout*        mainLayout;
    QScrollArea*        hdrScroll;

};

#endif // POI_PANEL_H
