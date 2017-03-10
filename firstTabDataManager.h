#ifndef FIRSTTABDATAMANAGER_H
#define FIRSTTABDATAMANAGER_H

#include "busline.h"
#include <QList>
#include <QStringList>

class FirstTabDataManager
{
private:
    BusMap *map;
    BuslineList allBuslines;
public:
    QList<Busline*> currentBuslines;
    StopList currentStops;
    FirstTabDataManager(BusMap *map);
    QStringList currentBusLineList();
    QStringList currentStopList();
    void searchTextChangedTo(const QString text);
    void stopSearchTextChangedTo(const QString text);
    void stopSelected(Stop *stop);
    void resetAllBuslines();
};

#endif // FIRSTTABDATAMANAGER_H
