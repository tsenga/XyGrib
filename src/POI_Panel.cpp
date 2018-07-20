#include "POI_Panel.h"

#include "POI_TableWidget.h"

POI_Panel::POI_Panel(QWidget *parent)
    : QDockWidget(parent)
{
    this->setObjectName("POIPanel");
    this->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    this->setWindowTitle(tr("Point of Interest"));

    this->dataTable = createDataTable();
}

void POI_Panel::refresh(GriddedPlotter* plotter)
{
    this->dataTable->refresh(plotter);
}

POI_TableWidget* POI_Panel::createDataTable()
{
    return new POI_TableWidget(this, GLOB_listSelectedPOI);
}

void POI_Panel::slotPOICreated(POI* poi)
{
    dataTable->refresh();
}

void POI_Panel::slotPOIDeleted(POI* poi)
{
    dataTable->refresh();
}

