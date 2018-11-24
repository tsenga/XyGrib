#ifndef POI_MANAGER_H
#define POI_MANAGER_H

#include <QList>
#include <QWidget>
#include <QMap>

#include "Projection.h"

class POI;

class POI_Manager : public QWidget
{
    Q_OBJECT

public:
    POI_Manager();

    void restoreBgOfSelectedPOIs();

    //void addPOI(POI *poi);
    //void removePOI(POI *poi);
    //void poiChanged(POI *poi);

    void unselectAll();

    void createPOIs(Projection *proj, QWidget *ownerSlotsPOI, QWidget *parentWindow);
    POI *createPOI(uint code, Projection *proj, QWidget *ownerSlotsPOI, QWidget *parentWindow);

    QMap<uint, POI*>::iterator begin();
    QMap<uint, POI*>::iterator end();

signals:
    void signalPOICreated(POI *poi);
    void signalPOIDeleted(POI *poi);
    void signalPOIChanged(POI *poi);

public slots:
    void selectPOI(POI *poi);
    void poiChanged(POI *poi);
    void addPOI(POI *poi);
    void removePOI(POI *poi);

protected:
    QList<POI*> selectedPOIs;
    QMap<uint, POI*> pois;

    void connectPOI(POI *poi);

};

#endif // POI_MANAGER_H
