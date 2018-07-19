#ifndef POI_TABLEWIDGET_H
#define POI_TABLEWIDGET_H

#include <QWidget>
#include <QGridLayout>

#include "GriddedPlotter.h"
#include "GriddedReader.h"

#include "POI.h"

class POI_TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit POI_TableWidget(QWidget *parent,
                             GriddedPlotter* plotter,
                             QList<POI*> listPOI);

signals:

public slots:

private:
        QWidget*        parent;
        QGridLayout*    layout;
        QGridLayout*    headerLayout;
        GriddedPlotter* plotter;
        GriddedReader*  reader;
        QList<POI*>     listPOI;
        QWidget*        headerWidget;

        void createTable();
};

#endif // POI_TABLEWIDGET_H
