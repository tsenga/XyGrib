#ifndef POI_PANEL_H
#define POI_PANEL_H

#include <QDockWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>

#include "GriddedPlotter.h"

class POI_Panel : public QDockWidget
{ Q_OBJECT
public:
    POI_Panel(QWidget *parent, GriddedPlotter *plotter);

private:
    GriddedPlotter* plotter;
    QWidget*        dataTable;
    QWidget*        createDataTable();
    QVBoxLayout*    mainLayout;
    QScrollArea*    hdrScroll;

};

#endif // POI_PANEL_H
