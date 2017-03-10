#ifndef BUSLINE
#define BUSLINE

#include <QString>
#include <QList>
#include <QStringList>
#include <QTime>
#include <QMap>
#include <QFile>
#include <QtDebug>
#include <iostream>

#define INFINITY -1
#define LINKED 1

class Busline;
class Stop;

typedef QList<Busline*> BuslineList;
typedef QList<Stop*> StopList;

class Stop {
public:
    QString name;
    BuslineList buslines;
};

class Busline {
public:
    StopList stops;
    QString name;
    QString busNumber, startStop, endStop;
    QTime firstTime, lastTime;
    QString ticket;
    Busline(const QString str, const QMap<QString, Stop*> stopMap);
    Busline();
};

class BusMap {

public:
    BuslineList buslines;
    QMap<QString, Stop*> stops;
    ~BusMap();
    BusMap(const QString dataFileName, const QString stopsFileName);
    StopList getStopList();
    Stop *stopForName(QString name);
    int **matrix;
};

class QueryResult {
public:
    StopList transStops;
    BuslineList transLines;
    int totalStopCount;
};

#endif // BUSLINE

