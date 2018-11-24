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

#ifndef METEOTABLEWIDGET_H
#define METEOTABLEWIDGET_H

#include <QLabel>

#include "MeteoWidget.h"


//===================================================================
// 
class MTGribData {
	public :
		MTGribData (QString codeStr, int pos) {
			this->codeStr = codeStr;
			this->pos = pos;
			this->dtc.fromInt32 (codeStr.toUInt(0, 16));
		}
		MTGribData (uint code, int pos) {
			this->codeStr = QString("%1").arg(code,8,16,QLatin1Char('0'));
			this->pos = pos;
			this->dtc.fromInt32 (codeStr.toUInt(0, 16));
		}
		QString  codeStr;
		DataCode dtc;
		int  pos;
};

//===================================================================
class MeteoTableWidget : public MeteoWidget
{ Q_OBJECT
    public:
        MeteoTableWidget (GriddedPlotter *plotter, 
						  double lon, double lat, 
						  QString locationName, QWidget *parent);
        ~MeteoTableWidget();
		QList <MTGribData *> & getListVisibleData() 
									{ return listVisibleData; }
		
		std::vector <DataPointInfo *> & getListPinfos() 
									{ return lspinfos; }

    protected:
        virtual bool showWindArrows();
        virtual bool showCurrentArrows();

		
	private:
        double          lon, lat;
		QString         locationName;
		
		std::vector <time_t> lsdates;
		std::vector <DataPointInfo *> lspinfos;
		
		QList <MTGribData *> listVisibleData;
		
		void  createTable();
		void  createListVisibleGribData();
		
		void  addLine_Wind        (const Altitude &alt, int lig);
		void  addLine_Current     (const Altitude &alt, int lig);
		void  addLine_Temperature (const Altitude &alt, uchar type, int lig);
		void  addLine_HumidRel    (const Altitude &alt, int lig);
		void  addLine_CloudCover  (int lig);
		void  addLine_Rain        (int lig);
		void  addLine_DewPoint    (const Altitude &alt, int lig);
		void  addLine_Pressure    (const Altitude &alt, int lig);
		void  addLine_SnowDepth        (int lig);
		void  addLine_DeltaTemperature (const Altitude &alt, uchar type, int lig);
		void  addLine_Categorical 	   (uchar type, int lig);
		void  addLine_CAPEsfc    (int lig);
        void  addLine_CINsfc     (int lig);
        // added by david
        void  addLine_Reflectivity     (int lig);
        void  addLine_SkewT      (int lig);
		void  addLine_GUSTsfc    (int lig);
		void  addLine_Isotherm0Height  (int lig);
		void  addLine_GeopotentialAltitude  (const Altitude &alt, int lig);

		void  addLine_WaveHeight (int type, int lig);
		void  addLine_WaveWhitecap (int type, int lig);
		void  addLine_WaveCompleteCell (int prvtype, int lig);
		

		

		

};




#endif
