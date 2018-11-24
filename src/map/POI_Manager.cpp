#include "POI_Manager.h"

#include "Util.h"
#include "Settings.h"
#include "POI.h"

#include <iostream>

POI_Manager::POI_Manager() : QWidget(NULL)
{

}

QMap<uint, POI*>::iterator POI_Manager::begin()
{
    return pois.begin();
}

QMap<uint, POI*>::iterator POI_Manager::end()
{
    return pois.end();
}

void POI_Manager::restoreBgOfSelectedPOIs()
{
    QList<POI*>::iterator iterPOI;

    for (iterPOI = selectedPOIs.begin(); iterPOI != selectedPOIs.end(); iterPOI++) {
        if ( (*iterPOI)->isValid() ) {
            (*iterPOI)->restoreBgColor();
            (*iterPOI)->update();
        }
    }
}

void POI_Manager::unselectAll()
{
    restoreBgOfSelectedPOIs();

    selectedPOIs.clear();
}

void POI_Manager::selectPOI(POI *poi)
{
    std::cerr << "POI_Manager::selectPOI" << std::endl;

    selectedPOIs.append(poi);
}

void POI_Manager::poiChanged(POI *poi)
{
    std::cerr << "POI_Manager::changedPOI" << std::endl;


    emit signalPOIChanged(poi);
}

// register POI with the manager
void POI_Manager::addPOI(POI *poi)
{
    std::cerr << "POI_Manager::addPOI" << std::endl;


    connectPOI(poi);

    pois[poi->getCode()] = poi;

    emit signalPOICreated(poi);
}

void POI_Manager::removePOI(POI *poi)
{
    std::cerr << "POI_Manager::removePOI" << std::endl;


    pois.remove(poi->getCode());

    emit signalPOIDeleted(poi);
}

void POI_Manager::createPOIs(Projection *proj, QWidget *ownerSlotsPOI, QWidget *parentWindow)
{
    QList<uint> lscodes = Settings::getSettingAllCodesPOIs();
    for (int i=0; i < lscodes.size(); ++i)
    {
        uint code = lscodes.at(i);

        POI* newPOI = createPOI(code, proj, ownerSlotsPOI, parentWindow);

        addPOI(newPOI);
    }
}

POI *POI_Manager::createPOI(uint code, Projection *proj, QWidget *ownerSlotsPOI, QWidget *parentWindow)
{
    POI *poi = new POI(this, code, proj, ownerSlotsPOI, parentWindow);

    bool visible = Util::getSetting("showPOIs", true).toBool();

    if (poi!=NULL) {
        if (poi->isValid()) {
            poi->setVisible(visible);
            this->addPOI(poi);

        } else {
            Settings::deleteSettingsPOI(poi->getCode());
            delete poi;
            poi = NULL;
        }
    }

    return poi;
}

void POI_Manager::connectPOI(POI *poi)
{
    connect(poi, SIGNAL(signalPOIselected(POI*)), this, SLOT(selectPOI(POI*)));
}
