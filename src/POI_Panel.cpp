#include "POI_Panel.h"

#include "POI_TableWidget.h"

#include <iostream>

#include <QScrollArea>

POI_Panel::POI_Panel(POI_Manager *poiManager, QWidget *parent)
    : QDockWidget(parent)
{
    this->poiManager = poiManager;

    this->setObjectName("poip");
    this->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    this->setWindowTitle(tr("Point of Interest"));

    dataTable = NULL;
}

void POI_Panel::setPlotter(GriddedPlotter *plotter)
{
    this->plotter = plotter;

    createDataTable();
    this->setWidget(dataTable);
}

QWidget* POI_Panel::createDataTable()
{
    if (dataTable) {
        delete dataTable;
    }

    poiTable = new POI_TableWidget(this, poiManager, plotter);
    assert (poiTable);
    poiTable->setObjectName("poitw");
    poiTable->setProperty("objectType", "POI_Panel");

    this->setWindowTitle(poiTable->property("objectType").toString());

    //-------------------------------------
    // scrollarea containing data area
    scrollArea = new QScrollArea();
    assert (scrollArea);
    scrollArea->setWidget (poiTable);
    scrollArea->setVerticalScrollBarPolicy   (Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

    //-------------------------------------
    // Headers area
    dataTable = new QWidget (this);
    assert (dataTable);

    // hdr column
    QWidget * hdrTable;
    hdrTable = new QWidget (dataTable);

    QVBoxLayout *layhdr = new QVBoxLayout (hdrTable);
    layhdr->setContentsMargins (0,0,0,0);
    layhdr->setSpacing (0);
        layhdr->addWidget (poiTable->getDataHeaders());
        layhdr->addStretch ();

    hdrScroll = new QScrollArea ();
    assert (hdrScroll);
    hdrScroll->setWidgetResizable (true);

    hdrScroll->setWidget (hdrTable);
    hdrScroll->setVerticalScrollBarPolicy   (Qt::ScrollBarAlwaysOff);
    hdrScroll->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOn);

    // synchronise les scrollbars
    QScrollBar * dataVscrollbar = scrollArea->verticalScrollBar ();
    QScrollBar * hdrVscrollbar = hdrScroll->verticalScrollBar ();

    connect (dataVscrollbar, SIGNAL(valueChanged(int)),
             hdrVscrollbar,  SLOT(setValue(int)));

    //-------------------------------------
    // hdr + data
    QGridLayout *laytable = new QGridLayout (dataTable);
    laytable->setContentsMargins (0,0,0,0);
    laytable->setSpacing (0);
        laytable->addWidget (hdrScroll, 0,0);
        laytable->addWidget (scrollArea, 0,1);

    laytable->setColumnStretch (0, 0);
    laytable->setColumnStretch (1, 1);

    // connect to POI updates from the manager

    connect(poiManager, SIGNAL(signalPOICreated(POI*)), this, SLOT(slotPOICreated(POI*)));
    connect(poiManager, SIGNAL(signalPOIDeleted(POI*)), this, SLOT(slotPOIDeleted(POI*)));
    connect(poiManager, SIGNAL(signalPOIChanged(POI*)), this, SLOT(slotPOIChanged(POI*)));

    return dataTable;
}

void POI_Panel::slotPOICreated(POI* poi)
{
    createDataTable();
    this->setWidget(dataTable);
    //poiTable->refresh();
}

void POI_Panel::slotPOIDeleted(POI* poi)
{
    createDataTable();
     this->setWidget(dataTable);
    //poiTable->refresh();
}

void POI_Panel::slotPOIChanged(POI* poi)
{
    createDataTable();
    this->setWidget(dataTable);
    //poiTable->refresh();
}


