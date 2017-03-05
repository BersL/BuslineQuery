#ifndef THIRDTABDATAMANAGER_H
#define THIRDTABDATAMANAGER_H

#include "busline.h"
#include <QStringList>


class ThirdTabDataManager
{
public:
    ThirdTabDataManager(BusMap *map);
    QStringList startStopList();
    QStringList endStopList();
    void startStopSearchTextChangedTo(QString text);
    void endStopSearchTextChangedTo(QString text);
    Stop* getStartStop(int index);
    Stop* getEndStop(int index);

    Stop *currentStartStop, *currentEndStop;

    QList<QueryResult*> queryBusline();

private:
    BusMap *map;
    StopList filteredStartStopList;
    StopList filteredEndStopList;
};



#endif // THIRDTABDATAMANAGER_H
