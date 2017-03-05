#include "busline.h"
#include <time.h>

int maxCount = 0;

Busline::Busline(const QString str, const QMap<QString, Stop*> stopMap) {
    QStringList list = str.split('*');
    this->name = list.at(0);

    this->busNumber = this->name.section('-', 0, 0);
    this->startStop = this->name.section('-', 1, 1);
    this->endStop = this->name.section('-', 2, 2);
    startStop = startStop.right(startStop.length() - 1);
    endStop = endStop.left(endStop.length() - 2);

    QString first = list.at(1).section('-', 0 ,0);
    int firstHour = first.section(':', 0, 0).toInt();
    int firstMinute = first.section(':', 1, 1).toInt();
    QString last = list.at(1).section('-', 1, 1);
    int lastHour = last.section(':', 0, 0).toInt();
    int lastMinute = last.section(':', 1, 1).toInt();
    this->firstTime = QTime(firstHour, firstMinute);
    this->lastTime = QTime(lastHour, lastMinute);

    this->ticket = list.at(2);

    QStringList::Iterator it = list.begin() + 3, end = list.end();
    for(; it != end; ++it) {
        Stop *stop = stopMap[*it];
        if (stop == NULL) {
            qDebug()<<"Stop "<<*it<<" Not Found In Map";
            continue;
        }
        this->stops<<stop;
        stop->buslines.append(this);
    }
}

Busline::Busline() {}

BusMap::BusMap(const QString dataFileName, const QString stopsFileName) {
    QFile stopsFile(stopsFileName);
    stopsFile.open(QIODevice::ReadOnly);
    QString fileText = stopsFile.readAll();
    QStringList stopsFileLines = fileText.split('\n', QString::SkipEmptyParts);
    foreach(QString line, stopsFileLines) {
        Stop *stop = new Stop;
        QStringList infoList = line.split('*');
        stop->name = infoList.at(0);
        this->stops[stop->name] = stop;
    }
    stopsFile.close();

    QFile dataFile(dataFileName);
    dataFile.open(QIODevice::ReadOnly);
    fileText = dataFile.readAll();
    QStringList dataFileLines = fileText.split('\n', QString::SkipEmptyParts);
    foreach(QString line, dataFileLines) {
        Busline *busLine = new Busline(line, this->stops);
        this->buslines<<busLine;
    }
    dataFile.close();

    int count = this->stops.count();
    this->matrix= new int *[count];
    for(int i = 0; i < count; ++i) {
        this->matrix[i] = new int[count];
        memset(this->matrix[i], INFINITY, sizeof(int)*count);
    }

    foreach(Busline *line, this->buslines) {
        StopList lineStops = line->stops;
        StopList allStops = this->getStopList();
        for(int i = 0; i<lineStops.count() - 1; ++i) {
            Stop *stop = lineStops[i], *nextStop = lineStops[i+1];
            int stopIndex = allStops.indexOf(stop),
                nextStopIndex = allStops.indexOf(nextStop);
            this->matrix[stopIndex][nextStopIndex] = LINKED;
        }
    }
}

StopList BusMap::getStopList() {
    StopList list = this->stops.values();
    std::reverse(list.begin(), list.end());
    return list;
}

Stop* BusMap::stopForName(QString name) {
    return this->stops[name];
}

BusMap::~BusMap() {
    qDeleteAll(this->buslines);
    this->buslines.clear();
    qDeleteAll(this->stops);
    this->stops.clear();
}
