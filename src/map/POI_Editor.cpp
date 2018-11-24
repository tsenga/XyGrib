/**********************************************************************
XyGrib: meteorological GRIB file viewer
Copyright (C) 2008-2012 - Jacques Zaninetti - http://www.zygrib.org

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include <cmath>
#include <cassert>
#include <iostream>

#include <QTimer>
#include <QMessageBox>

#include "Settings.h"
#include "POI_Editor.h"

//-------------------------------------------------------
// POI_Editor: Constructor for edit and create a new POI
//-------------------------------------------------------
POI_Editor::POI_Editor(POI_Manager *poiManager_, uint code, double lon, double lat,
				Projection *proj, QWidget *ownerMeteotable, QWidget *parentWindow)
	: QDialog(parentWindow)
{
    assert(poiManager_);

	setupUi(this);
	modeCreation = true;
	setWindowTitle(tr("New Point of interest"));
    this->poiManager = poiManager_;
    this->poi = new POI(poiManager_, code, tr("Point %1").arg(code), lon, lat, proj, ownerMeteotable, parentWindow);
	assert(this->poi);
	updateInterface();
}
//-------------------------------------------------------
// POI_Editor: Constructor for edit an existing POI
//-------------------------------------------------------
POI_Editor::POI_Editor(POI_Manager *poiManager_, POI *poi_, QWidget *parent)
	: QDialog(parent)
{
    assert(poiManager_);

	setupUi(this);
	setModal(false);
    this->poiManager = poiManager_;
	this->poi = poi_;
	modeCreation = false;
	setWindowTitle(tr("Point of interest: ")+poi->getName());
	updateInterface();
}
//---------------------------------------
POI_Editor::~POI_Editor()
{
	//printf("delete POI_Editor\n");
}
//---------------------------------------
void POI_Editor::updateInterface()
{
	inputName->setText (poi->name);
	
	inputStyle->setFont (poi->labelFont);
	inputStyle->setTextColor (poi->labelTextColor);
	QColor bgColor;
	// read correct bg-color of POI, not the one used to mark a selected POI, TH20110103
	if (poi->labelBgColorMarkedPOI.isValid()) {
		bgColor = poi->labelBgColorMarkedPOI;
	} else {
		bgColor = poi->labelBgColor;
	}
	bgColor.setAlpha(255);	// no transparency in color editor
	inputStyle->setBgColor (bgColor);
	
	inputPosition->setPosition (poi->lat, poi->lon);
	
	//printf ("markColor: %s\n", qPrintable(poi->markColor.name()) );
	inputMarkColor->setColor (poi->markColor);
	inputMarkColor->setDefaultColor (Qt::red);
	
	cbIsMovable->setChecked (poi->isMovable);
	cbShowLabel->setChecked (poi->showLabel);
	
	connect(btValid,  SIGNAL(clicked()), this, SLOT(btOkClicked()));
	connect(btCancel, SIGNAL(clicked()), this, SLOT(btCancelClicked()));
	connect(btDelete, SIGNAL(clicked()), this, SLOT(btDeleteClicked()));

    connect(this, SIGNAL(signalPOIChanged(POI*)), poiManager, SLOT(poiChanged(POI*)));
    connect(this, SIGNAL(signalPOICreated(POI*)), poiManager, SLOT(addPOI(POI*)));
    connect(this, SIGNAL(signalPOIDeleted(POI*)), poiManager, SLOT(removePOI(POI*)));

	setModal(false);
	show();
}
//---------------------------------------
void POI_Editor::reject()
{
	btCancelClicked();
}
//---------------------------------------
void POI_Editor::btOkClicked()
{
 	QString name = (inputName->text()).trimmed();
 	if (name == "") {
		QMessageBox::critical (this,
					tr("POI"),  tr("Error: name is required.") );
		return;
 	}
	poi->setName(name);
	
	poi->setLongitude(inputPosition->getLongitude());
	poi->setLatitude (inputPosition->getLatitude());
	
	poi->setDisplayParams ( inputMarkColor->getColor(),
							inputStyle->getFont(),
							inputStyle->getTextColor(),
							inputStyle->getBgColor()
						);
	poi->adjustGeometry();
	
	poi->isMovable = cbIsMovable->isChecked();
	poi->showLabel = cbShowLabel->isChecked();

	poi->projectionUpdated();
	
	poi->writeSettings();	// save new pamameters to settings
	
	if (modeCreation) {
        //poiManager->addPOI(poi);

		poi->show();

        std::cerr << "POI_Editor::btOKClicked (creation)" << std::endl;

        emit signalPOICreated(poi);
    } else {
        //poiManager->poiChanged(poi);


        std::cerr << "POI_Editor::btOKClicked (change)" << std::endl;

        emit signalPOIChanged(poi);
    }
	delete this;	
}
//---------------------------------------
void POI_Editor::btCancelClicked()
{
	if (modeCreation) {
		delete poi;
	}
	delete this;
}
//---------------------------------------
void POI_Editor::btDeleteClicked()
{
	if (! modeCreation) {
		int rep = QMessageBox::question (this,
			tr("Delete POI: %1").arg(poi->getName()),
			tr("The destruction of a point of interest is definitive.\n\nAre you sure ?"),
			QMessageBox::Yes | QMessageBox::No);

		if (rep == QMessageBox::Yes)
		{
			Settings::deleteSettingsPOI(poi->getCode());            

            //poiManager->removePOI(poi);
            emit signalPOIDeleted(poi);

            std::cerr << "POI_Editor::btDeleteClicked" << std::endl;

			delete poi;
			delete this;
		}
	}
	else {
		delete poi;
		delete this;
	}
}




