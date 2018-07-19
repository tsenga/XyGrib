#include "POI_Panel.h"

#include "POI_TableWidget.h"

POI_Panel::POI_Panel(QWidget *parent, GriddedPlotter *plotter)
    : QDockWidget(parent)
{
    this->setObjectName("POIPanel");
    this->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    this->setWindowTitle(tr("Point of Interest"));

    this->plotter = plotter;

    this->dataTable = createDataTable();
}

QWidget* POI_Panel::createDataTable()
{
    QWidget* dataTable = new POI_TableWidget(this, this->plotter, GLOB_listSelectedPOI);

    return dataTable;
}
